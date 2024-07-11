//
// Created by LL06p on 24-7-6.
//

#include <concepts>
#include <ranges>

#include "utility/Logger.h"
#include "AST/SyntaxTreeBuilder.h"

namespace SysYust::AST {

    namespace ranges = std::ranges;
    namespace views = std::views;


    // 拷贝控制

    SyntaxTreeBuilder::SyntaxTreeBuilder(SysYParser::CompUnitContext *tree)
    : rawTree(tree)
    , v(*this) {

    }

    std::unique_ptr<SyntaxTree> SyntaxTreeBuilder::getTree() {
        if (rawTree) {
            rawTree = nullptr;
            currentEnv = nullptr;
            return std::move(tree);
        } else {
            return nullptr;
        }
    }

    std::unique_ptr<SyntaxTree> SyntaxTreeBuilder::getTree(SysYParser::CompUnitContext *tree) {
        SyntaxTreeBuilder builder(tree);
        return builder.getTree();
    }

    SyntaxTreeBuilder::Visitor::Visitor(SyntaxTreeBuilder &b)
    : global(b)
    , eval(*global.tree) {

    }

    // 工具函数

    const Type &SyntaxTreeBuilder::Visitor::toType(std::string_view type) {
        if (type == "void") {
            return Void_v;
        } else if (type == "int") {
            return Int_v;
        } else if (type == "float") {
            return Float_v;
        } else {
            /// @todo 添加一个匹配，使得允许某些类型确实

            LOG_ERROR("Unrecognised type {}", type);
            std::exit(EXIT_FAILURE);
        }
    }

    std::pair<HNode, HNode> SyntaxTreeBuilder::Visitor::numberTypeCast(HNode lhs, HNode rhs) {
        auto lhsType = global.tree->getNode<Expr>(lhs)->type;
        auto rhsType = global.tree->getNode<Expr>(rhs)->type;
        if (!match(*lhsType, *rhsType)) {
            assert(lhsType->isBasicType() && rhsType->isBasicType());
            HNode toFloatNode;
            HNode newNodeId = global.tree->pushNode();
            if (lhsType->type() == TypeId::Int) {
                assert(rhsType->type() == TypeId::Float);
                toFloatNode = lhs;
                lhs = newNodeId;
            } else if (rhsType->type() == TypeId::Int) {
                assert(lhsType->type() == TypeId::Float);
                toFloatNode = rhs;
                rhs = newNodeId;
            } else {
                LOG_ERROR("Unexpected type for lhs:{}, rhs:{}", lhsType->toString(), rhsType->toString());
                std::exit(EXIT_FAILURE);
            }
            auto newNode = new ToFloat(toFloatNode);
            global.tree->setNode(newNodeId, newNode);
        }
        return {lhs, rhs};
    }

    HNode SyntaxTreeBuilder::Visitor::convertTo(const Type &t, HNode n) {
        auto &nodeType = *global.tree->getNode<Expr>(n)->type;
        if (t.isBasicType()) { // Float 与 Int 之间的转换
            assert(nodeType.isBasicType());
            if (match(t, nodeType)) {
                return n;
            } else {
                if (t.type() == TypeId::Int) {
                    auto toId = global.tree->pushNode();
                    auto toNode = new ToInt(n);
                    global.tree->setNode(toId, toNode);
                    return toId;
                } else if (t.type() == TypeId::Float) {
                    auto toId = global.tree->pushNode();
                    auto toNode = new ToFloat(n);
                    global.tree->setNode(toId, toNode);
                    return toId;
                } else {
                    LOG_TRACE("TRACE info t:{} , node type: {}", t.toString(), nodeType.toString());
                    LOG_ERROR("Reach to unreachable pointed");
                    std::exit(EXIT_FAILURE);
                }
            }
        } else { // 指针与数组的转换
            assert(nodeType.type() == TypeId::Array);
            assert(t.type() == TypeId::Pointer);
            if (match(t, nodeType)) {
                return n; /// @todo 待决，是否应该添加一个将数组转换为指针的类型转换
            } else {
                LOG_ERROR("Unmatched type cast: from {} to {}", nodeType.toString(), t.toString());
                std::exit(EXIT_FAILURE);
            }
        }
    }

    HNode SyntaxTreeBuilder::Visitor::convertToCond(HNode n) {
        auto node = global.tree->getNode(n);
        if (dynamic_cast<CondExpr*>(node)) {
            auto condNode = new ToCond(n);
            auto nodeId = global.tree->pushNode(condNode);
            return nodeId;
        } else {
            return n;
        }
    }

    // 节点构建

    std::any SyntaxTreeBuilder::Visitor::visitCompUnit(SysYParser::CompUnitContext *ctx) {
        for (auto decl : ctx->children) {
            decl->accept(this);
        }
        return nullptr;
    }

    // 常量声明
    std::any SyntaxTreeBuilder::Visitor::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
        // 查询基础类型
        auto &baseType = toType(ctx->type()->getText());

        std::vector<HNode> declNodes;

        // 展开每个定义
        for (auto &def : ctx->constDef()) {
            auto indexDef = def->constExp();

            // 计算类型
            const Type *type;
            if (indexDef.empty()) {
                // 非数组类型
                type = &baseType;
            } else {
                // 计算各维长度
                auto indexNode = indexDef
                | views::transform([&](auto i) {
                    return std::any_cast<HNode>(i->accept(this));
                });
                auto indexNum = indexNode
                | views::reverse
                | views::transform([&](auto i) {
                    return get<std::int32_t>(eval.compute(i));
                });

                // 创建类型
                type = &Array::create(baseType, {indexNum.begin(), indexNum.end()});
            }

            // 进入符号表
            auto varName = def->Ident()->getText();
            auto varId = global.currentEnv->getId(varName);
            VarInfo info;
            info.type = type;
            info.decl = global.tree->pushNode();
            // 构建节点
            auto varNode = new VarDecl();
            varNode->info_id = varId;
            varNode->init_expr = std::any_cast<HNode>(def->constInitVal()->accept(this));
            global.tree->setNode(info.decl, varNode);
            // 节点构建结束
            info.isConstant = true;
            info.isParam = false;
            global.currentEnv->var_table.setInfo(varId, info);
            declNodes.push_back(info.decl);
        }
        return declNodes;
    }

    // 常量初始化

    // 变量声明
    template<typename T>
    concept VarDefContext = std::same_as<T, SysYParser::UninitVarDefContext> ||
    std::same_as<T, SysYParser::InitVarDefContext>;
    std::any SyntaxTreeBuilder::Visitor::visitVarDecl(SysYParser::VarDeclContext *ctx) {
        auto &baseType = toType(ctx->type()->getText());
        std::vector<HNode> varNodes;
        // 通过这个Lambda表达式复用一些代码
        auto impl = [&](VarDefContext auto &def_ctx) {
            // 计算类型
            const Type *type;
            auto indexDef = def_ctx.constExp();
            if (indexDef.empty()) {
                type = &baseType;
            } else {
                auto indexNum = indexDef
                | views::transform([&](auto i) {
                    return std::any_cast<HNode>(i->accept(this));
                })
                | views::reverse
                | views::transform([&](auto i) {
                    return std::get<std::int32_t>(eval.compute(i));
                });
                type = &Array::create(baseType, {indexNum.begin(), indexNum.end()});
            }

            VarInfo info;
            info.name = def_ctx.Ident()->getText();
            auto infoId = global.currentEnv->getId(info.name);
            info.decl = global.tree->pushNode();
            info.type = type;
            info.isConstant = false;
            info.isParam = false;
            auto varNode = new VarDecl();
            if constexpr (std::same_as<decltype(ctx), SysYParser::InitVarDefContext>) {
                varNode->init_expr = std::any_cast<HNode>(def_ctx.initVal->accept(this));
            }
            varNode->info_id = infoId;
            global.tree->setNode(info.decl, varNode);
            global.currentEnv->var_table.setInfo(infoId, info);
            varNodes.push_back(info.decl);
        };
        for (auto def : ctx->varDef()) {
            if (typeid(def) == typeid(SysYParser::UninitVarDefContext)) {
                impl(*static_cast<SysYParser::UninitVarDefContext*>(def));
            } else {
                impl(*static_cast<SysYParser::InitVarDefContext*>(def));
            }
        }
        return varNodes;
    }


    // 初始化

    // 表达式计算

    // 直接引用计算

    std::any SyntaxTreeBuilder::Visitor::visitFromMul(SysYParser::FromMulContext *ctx) {
        auto subExpr = ctx->mulExp();
        return subExpr->accept(this);
    }

    std::any SyntaxTreeBuilder::Visitor::visitFromUnary(SysYParser::FromUnaryContext *ctx) {
        auto subExpr = ctx->unaryExp();
        return subExpr->accept(this);
    }

    std::any SyntaxTreeBuilder::Visitor::visitFromPrimary(SysYParser::FromPrimaryContext *ctx) {
        return ctx->primaryExp()->accept(this);
    }

    std::any SyntaxTreeBuilder::Visitor::visitClosedExpr(SysYParser::ClosedExprContext *ctx) {
        auto subExpr = ctx->exp();
        return subExpr->accept(this);
    }

    std::any SyntaxTreeBuilder::Visitor::visitLValValue(SysYParser::LValValueContext *ctx) {
        auto subExpr = ctx->lVal();
        return subExpr->accept(this);
    }

    std::any SyntaxTreeBuilder::Visitor::visitLiteralValue(SysYParser::LiteralValueContext *ctx) {
        auto subExpr = ctx->number();
        return subExpr->accept(this);
    }

    // 带有子表达式的

    std::any SyntaxTreeBuilder::Visitor::visitAddOp(SysYParser::AddOpContext *ctx) {

        // 计算左右运算元
        auto lhs = std::any_cast<HNode>(ctx->addExp()->accept(this));
        auto rhs = std::any_cast<HNode>(ctx->addExp()->accept(this));

        // 隐式类型转换
        std::tie(lhs, rhs) = numberTypeCast(lhs, rhs);

        auto op = ctx->op->getText().front();
        BinaryOp::OpType t;
        switch (op) {
            case '+':
                t = BinaryOp::Add;
                break;
            case '-':
                t = BinaryOp::Sub;
                break;
            default:
                LOG_ERROR("Unexpected operator {}", op);
                std::exit(EXIT_FAILURE);
        }

        auto type = global.tree->getNode<Expr>(lhs)->type;
        assert(type->isBasicType());
        auto newNodeId = global.tree->pushNode();
        auto newNode = new BinaryOp(type, t, lhs, rhs);
        global.tree->setNode(newNodeId, newNode);
        return newNodeId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitMulOp(SysYParser::MulOpContext *ctx) {
        // 左右操作元
        auto lhs = std::any_cast<HNode>(ctx->mulExp()->accept(this));
        auto rhs = std::any_cast<HNode>(ctx->unaryExp()->accept(this));

        // 类型转换
        std::tie(lhs, rhs) = numberTypeCast(lhs, rhs);

        // 操作符类型
        BinaryOp::OpType t;
        auto op = ctx->op->getText().front();
        switch (op) {
            case '*':
                t = BinaryOp::Mul;
                break;
            case '/':
                t = BinaryOp::Div;
                break;
            case '%':
                t = BinaryOp::Mod;
                break;
            default:
                LOG_ERROR("Unexpected operator {}", op);
                std::exit(EXIT_FAILURE);
        }

        // 表达式类型
        auto type = global.tree->getNode<Expr>(lhs)->type;
        assert(type->isBasicType());

        // 节点
        auto newNodeId = global.tree->pushNode();
        auto newNode = new BinaryOp(type, t, lhs, rhs);
        global.tree->setNode(newNodeId, newNode);
        return newNodeId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitCall(SysYParser::CallContext *ctx) {
        // 查询信息
        auto funcName = ctx->Ident()->getText();
        auto funcId = global.currentEnv->getId(funcName);
        auto &funcInfo = global.currentEnv->func_table.getInfo(funcId);
        auto &funcType = *funcInfo.type;
        auto &resultType = funcType.getResult();

        // 准备实参
        auto callNodeId = global.tree->pushNode();
        auto arguments = ctx->funcRParams()->exp();
        auto params = funcType.getParam();
        std::vector<HNode> argumentExprNodeId;
        if (params.size() == arguments.size()) {
            auto len = params.size();
            for (int i=0; i<len; ++i) {
                auto &currentParma = *params[i];
                auto &currentArgument = arguments[i];
                auto argId = std::any_cast<HNode>(currentArgument->accept(this));
                argId = convertTo(currentParma, argId);
                argumentExprNodeId.push_back(argId);
            }
        } else {
            LOG_ERROR("Unmatched function call for {}", funcName);
            std::exit(EXIT_FAILURE);
        }

        // 准备节点
        auto callId = global.tree->pushNode();
        auto callNode = new Call(&resultType, funcId, std::move(argumentExprNodeId));
        global.tree->setNode(callId, callNode);
        return callId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitOpUnary(SysYParser::OpUnaryContext *ctx) {

        // 计算子节点
        auto subexpr = std::any_cast<HNode>(ctx->unaryOP()->accept(this));

        auto op = ctx->unaryOP()->getText().front();
        if (op == '!') {
            auto notId = global.tree->pushNode();
            auto notNode = new Not(subexpr);
            global.tree->setNode(notId, notNode);
            return notId;
        } else if (op == '+') {
            return subexpr;
        } else {
            assert(op == '-');
            UnaryOp::OpType opT = UnaryOp::Negative;
            auto type = global.tree->getNode<Expr>(subexpr)->type;

            auto nId = global.tree->pushNode();
            auto nNode = new UnaryOp(type, opT, subexpr);
            global.tree->setNode(nId, nNode);
            return nId;
        }
    }

    std::any SyntaxTreeBuilder::Visitor::visitLVal(SysYParser::LValContext *ctx) {
        // 查询信息
        auto varName = ctx->Ident()->getText();
        auto varId = global.currentEnv->getId(varName);
        auto &varInfo = global.currentEnv->var_table.getInfo(varId);
        auto &varType = *varInfo.type;

        auto indexExpr = ctx->exp();

        if (indexExpr.empty()) { // 非数组引用

            auto refId = global.tree->pushNode();
            auto refNode = new DeclRef(&varType, varId);
            global.tree->setNode(refId, refNode);
            return refId;

        } else { // 数组引用

            // 计算子表达式
            auto subexpr = indexExpr | views::transform([&](auto i) {
               return std::any_cast<HNode>(i->accept(this));
            });
            HNode refId = global.tree->pushNode();
            ArrayRef *refNode;

            // 构造节点
            if (varType.type() == TypeId::Array) {
                auto &arrType = static_cast<const Array&>(varType);
                auto &targetType = arrType.index(indexExpr.size());
                refNode = new ArrayRef(&targetType, varId, {subexpr.begin(), subexpr.end()});
            } else if (varType.type() == TypeId::Pointer) {
                auto &ptrType = static_cast<const Pointer&>(varType);
                auto &targetType = ptrType.index(indexExpr.size());
                refNode = new ArrayRef(&targetType, varId, {subexpr.begin(), subexpr.end()});
            } else {
                LOG_ERROR("Unexpected type indexed: the type is {}", varType.toString());
                std::exit(EXIT_FAILURE);
            }

            global.tree->setNode(refId, refNode);
            return refId;

        }
    }

    std::any SyntaxTreeBuilder::Visitor::visitFloatNumber(SysYParser::FloatNumberContext *ctx) {
        auto lit = ctx->FloatConst()->getText();
        auto num  = std::stof(lit);
        auto litId = global.tree->pushNode();
        auto litNode = new FloatLiteral(num);
        global.tree->setNode(litId, litNode);
        return litId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitIntNumber(SysYParser::IntNumberContext *ctx) {
        auto lit = ctx->IntConst()->getText();
        auto num = std::stoi(lit);
        auto litId = global.tree->pushNode();
        auto litNode = new IntLiteral(num);
        global.tree->setNode(litId, litNode);
        return litId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitRelOp(SysYParser::RelOpContext *ctx) {
        auto lhsId = std::any_cast<HNode>(ctx->relExp()->accept(this));
        auto rhsId = std::any_cast<HNode>(ctx->addExp()->accept(this));

        lhsId = convertToCond(lhsId);
        rhsId = convertToCond(rhsId);

        Compare::CompareType t;
        auto op = ctx->op->getText();
        if (op == "<") {
           t = Compare::LT;
        } else if (op == "<=") {
            t = Compare::LE;
        } else if (op == ">") {
            t = Compare::GT;
        } else if (op == ">=") {
            t = Compare::GE;
        } else {
            LOG_ERROR("Unexpected operator({}) for compare", op);
            std::exit(EXIT_FAILURE);
        }

        auto relId = global.tree->pushNode();
        auto relNode = new Compare(t, lhsId, rhsId);
        global.tree->setNode(relId, relNode);
        return relId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitEqOp(SysYParser::EqOpContext *ctx) {
        auto lhsId = std::any_cast<HNode>(ctx->eqExp()->accept(this));
        auto rhsId = std::any_cast<HNode>(ctx->relExp()->accept(this));

        lhsId = convertToCond(lhsId);
        rhsId = convertToCond(rhsId);

        auto op = ctx->op->getText();
        Compare::CompareType t;
        if (op == "==") {
            t = Compare::EQ;
        } else if (op == "!=") {
            t = Compare::NE;
        } else {
            LOG_ERROR("Unexpected operator({}) for compare", op);
            std::exit(EXIT_FAILURE);
        }

        auto relId = global.tree->pushNode();
        auto relNode = new Compare(t, lhsId, rhsId);
        global.tree->setNode(relId, relNode);
        return relId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitLAndOp(SysYParser::LAndOpContext *ctx) {
        auto lhsId = std::any_cast<HNode>(ctx->lAndExp()->accept(this));
        auto rhsId = std::any_cast<HNode>(ctx->eqExp()->accept(this));

        lhsId = convertToCond(lhsId);
        rhsId = convertToCond(rhsId);

        return global.tree->pushNode(new And(lhsId, rhsId));
    }

    std::any SyntaxTreeBuilder::Visitor::visitLOrOp(SysYParser::LOrOpContext *ctx) {
        auto lhsId = std::any_cast<HNode>(ctx->lOrExp()->accept(this));
        auto rhsId = std::any_cast<HNode>(ctx->lAndExp()->accept(this));

        lhsId = convertToCond(lhsId);
        rhsId = convertToCond(rhsId);

        return global.tree->pushNode(new Or(lhsId, rhsId));
    }

} // AST