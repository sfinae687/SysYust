//
// Created by LL06p on 24-7-6.
//

#include <limits>
#include <type_traits>

#include "AST/Node/Expr.h"
#include "AST/Type/Float.h"
#include "utility/Logger.h"
#include "AST/SyntaxTreeBuilder.h"

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
namespace views = ranges::views;


namespace SysYust::AST {

    std::set<FuncInfo> SyntaxTreeBuilder::lib_funcs;

    std::set<NumId> SyntaxTreeBuilder::lib_funcs_id{};

    // 拷贝控制

    SyntaxTreeBuilder::SyntaxTreeBuilder(SysYParser::CompUnitContext *tree)
    : rawTree(tree)
    , v(*this) {
        lib_funcs = {
            {
                "getint",
                &Function::create(Int_v, {&Void_v}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "getch",
                &Function::create(Int_v, {&Void_v}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "getarray",
                &Function::create(Int_v, {&Pointer::create(Int_v)}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "getfloat",
                &Function::create(Float_v, {&Void_v}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "getfarray",
                &Function::create(Int_v, {&Pointer::create(Float_v)}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "putint",
                &Function::create(Void_v, {&Int_v}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "putch",
                &Function::create(Void_v, {&Int_v}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "putarray",
                &Function::create(Void_v, {&Int_v, &Pointer::create(Int_v)}),
                std::numeric_limits<HNode>::max(),
            },
            {
                "putfloat",
                        &Function::create(Void_v, {&Float_v}),
                        std::numeric_limits<HNode>::max(),
            },
            {
                "putfarray",
                &Function::create(Void_v, {&Int_v, &Pointer::create(Float_v)}),
                std::numeric_limits<HNode>::max(),
            },
            {
                    "putf",
                    nullptr,
                    std::numeric_limits<HNode>::max(),
            },
            {
                "_sysy_starttime",
                &Function::create(Void_v, {&Int_v}),
                std::numeric_limits<HNode>::max(),
            },
            {
                    "_sysy_stoptime",
                    &Function::create(Void_v, {&Int_v}),
                    std::numeric_limits<HNode>::max(),
            },
    };
        auto &funcTable = currentEnv->func_table;
        for (auto &i : lib_funcs) {
            auto id = currentEnv->getId(i.name);
            lib_funcs_id.insert(id);
            funcTable.setInfo(id, i);
        }
    }

    std::unique_ptr<SyntaxTree> SyntaxTreeBuilder::getTree() {
        if (rawTree) {
            rawTree->accept(&v);
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
        if (type == "int") {
            return Int_v;
        } else if (type == "float") {
            return Float_v;
        } else if (type == "void") {
            return Void_v;
        } else{
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
        if (typeid(*global.tree->getNode(n)) == typeid(List)) {
            return n; // 忽略List
        }
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
                    LOG_ERROR("Reach to unreachable pointed");
                    std::exit(EXIT_FAILURE);
                }
            }
        } else { // 指针与数组的转换
            assert(nodeType.type() == TypeId::Array || nodeType.type() == TypeId::Pointer);
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
        if (!dynamic_cast<CondExpr*>(node)) {
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
            LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
            decl->accept(this);
        }
        return nullptr;
    }

    // 常量声明
    std::any SyntaxTreeBuilder::Visitor::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        // 查询基础类型
        auto &baseType = toType(ctx->type()->getText());
        _current_expr_basic_type = &baseType;

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
            info.name = varName;
            info.type = type;
            info.decl = global.tree->pushNode();
            // 构建节点
            auto varNode = new VarDecl();
            varNode->info_id = varId;
            varNode->init_expr = convertTo(*type, std::any_cast<HNode>(def->constInitVal()->accept(this)));
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
    std::any SyntaxTreeBuilder::Visitor::visitVarDecl(SysYParser::VarDeclContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto &baseType = toType(ctx->type()->getText());
        _current_expr_basic_type = &baseType;
        std::vector<HNode> varNodes;
        // 通过这个Lambda表达式复用一些代码
        auto impl = [&](auto &def_ctx) {
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

            if constexpr (std::same_as<std::remove_cvref_t<decltype(def_ctx)>, SysYParser::InitVarDefContext>) {
                varNode->init_expr = convertTo(*type, std::any_cast<HNode>(def_ctx.initVal()->accept(this)));
            }
            varNode->info_id = infoId;
            global.tree->setNode(info.decl, varNode);
            global.currentEnv->var_table.setInfo(infoId, info);
            varNodes.push_back(info.decl);
        };
        //NOLINTBEGIN(cppcoreguidelines-pro-type-static-cast-downcast)
        for (auto def : ctx->varDef()) {
            if (typeid(*def) == typeid(SysYParser::UninitVarDefContext)) {
                impl(*static_cast<SysYParser::UninitVarDefContext*>(def));
            } else {
                impl(*static_cast<SysYParser::InitVarDefContext*>(def));
            }
        }
        //NOLINTEND(cppcoreguidelines-pro-type-static-cast-downcast)
        return varNodes;
    }


    // 初始化
    std::any SyntaxTreeBuilder::Visitor::visitConstListInit(SysYParser::ConstListInitContext *ctx) {
        auto initVal = ctx->constInitVal();
        auto vaList = initVal
        | views::transform([&](auto i) {
            return convertTo(*_current_expr_basic_type, std::any_cast<HNode>(i->accept(this)));
        });
        auto nodeId = global.tree->pushNode();
        auto node = new List(std::vector(vaList.begin(), vaList.end()));
        global.tree->setNode(nodeId, node);
        return nodeId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitListInit(SysYParser::ListInitContext *ctx) {
        auto initVal = ctx->initVal();
        auto vaList = initVal
        | views::transform([&](auto i) {
            return convertTo(*_current_expr_basic_type, std::any_cast<HNode>(i->accept(this)));
        });
        auto nodeId = global.tree->pushNode();
        auto node = new List(std::vector(vaList.begin(), vaList.end()));
        global.tree->setNode(nodeId, node);
        return nodeId;
    }

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

    std::any SyntaxTreeBuilder::Visitor::visitExpr(SysYParser::ExprContext *ctx) {
        auto expCtx = ctx->exp();
        if (expCtx) {
            return ctx->exp()->accept(this);
        } else {
            return global.tree->pushNode(new Empty());
        }
    }

    // 带有子表达式的

    std::any SyntaxTreeBuilder::Visitor::visitAddOp(SysYParser::AddOpContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());

        // 计算左右运算元
        auto lhs = std::any_cast<HNode>(ctx->addExp()->accept(this));
        auto rhs = std::any_cast<HNode>(ctx->mulExp()->accept(this));

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
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
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
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        // 查询信息
        auto funcName = ctx->Ident()->getText();

        // 检查是否是时间库
        if (funcName == "starttime" || funcName == "stoptime") {
            return processTimeLib(ctx);
        }

        auto funcId = global.currentEnv->getId(funcName);
        auto &funcInfo = global.currentEnv->func_table.getInfo(funcId);
        auto &funcType = *funcInfo.type;
        auto &resultType = funcType.getResult();

        // 准备实参
        auto callNodeId = global.tree->pushNode();

        auto argumentList = ctx->funcRParams();
        std::vector<HNode> argumentExprNodeId;

        if (argumentList) {
            auto arguments = argumentList->exp();

            auto params = funcType.getParam();
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
        }

        // 准备节点
        auto callNode = new Call(&resultType, funcId, std::move(argumentExprNodeId));
        global.tree->setNode(callNodeId, callNode);
        return callNodeId;
    }

    HNode SyntaxTreeBuilder::Visitor::processTimeLib(SysYParser::CallContext *ctx) {
        auto lineno = ctx->getStart()->getLine();
        auto funcName = ctx->Ident()->getText();
        auto fullName = "_sysy_" + funcName;
        auto funcId = global.currentEnv->getId(funcName);

        auto callId = global.tree->pushNode();
        auto argument = global.tree->pushNode(new IntLiteral(lineno));
        auto callNode = new Call(&Void_v, funcId, {argument});
        global.tree->setNode(callId, callNode);
        return callId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitOpUnary(SysYParser::OpUnaryContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());

        // 计算子节点
        auto subexpr = std::any_cast<HNode>(ctx->unaryExp()->accept(this));

        auto op = ctx->unaryOP()->getText().front();
        if (op == '!') {
            auto notId = global.tree->pushNode();
            subexpr = convertToCond(subexpr);
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
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
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
            // NOLINTBEGIN(cppcoreguidelines-pro-type-static-cast-downcast)
            if (varType.type() == TypeId::Array) {
                //
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
            // NOLINTEND(cppcoreguidelines-pro-type-static-cast-downcast)

            global.tree->setNode(refId, refNode);
            return refId;

        }
    }

    std::any SyntaxTreeBuilder::Visitor::visitFloatNumber(SysYParser::FloatNumberContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto lit = ctx->FloatConst()->getText();
        auto num  = std::stof(lit);
        auto litId = global.tree->pushNode();
        auto litNode = new FloatLiteral(num);
        global.tree->setNode(litId, litNode);
        return litId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitIntNumber(SysYParser::IntNumberContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto lit = ctx->IntConst()->getText();
        auto num = std::stoi(lit, nullptr, 0);
        auto litId = global.tree->pushNode();
        auto litNode = new IntLiteral(num);
        global.tree->setNode(litId, litNode);
        return litId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitRelOp(SysYParser::RelOpContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto lhsId = std::any_cast<HNode>(ctx->relExp()->accept(this));
        auto rhsId = std::any_cast<HNode>(ctx->addExp()->accept(this));

        // lhsId = convertToCond(lhsId);
        // rhsId = convertToCond(rhsId);

        std::tie(lhsId, rhsId) = numberTypeCast(lhsId, rhsId);

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

        // lhsId = convertToCond(lhsId);
        // rhsId = convertToCond(rhsId);

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

    // 函数声明

    std::any SyntaxTreeBuilder::Visitor::visitFuncDef(SysYParser::FuncDefContext *ctx) {
        FuncInfo info;
        auto funcDeclId = global.tree->pushNode();
        auto funcDeclNode = new FuncDecl;

        info.name = ctx->Ident()->getText();
        info.node = funcDeclId;

        auto nameId = global.currentEnv->getId(info.name);
        funcDeclNode->info_id = nameId;

        auto &resultType = toType(ctx->type()->getText());
        _returned_type = &resultType;

        // 处理形参并获取形参类型
        global.currentEnv = global.tree->pushEnv();
        auto params = ctx->funcFParams();
        const Function *type;
        if (params) { // 计算并设置形参声明的节点
            funcDeclNode->param = std::any_cast<std::vector<HNode>>(params->accept(this));
            // 计算参数类型
            auto paramTypes = funcDeclNode->param
                              | views::transform([&](auto i) {
                auto paramId = global.tree->getNode<ParamDecl>(i)->info_id;
                return global.currentEnv->var_table.getInfo(paramId).type;
            });

            type = &Function::create(resultType, {paramTypes.begin(), paramTypes.end()});
        } else {
            type = &Function::create(resultType, {&Void_v});
        }
        info.type = type;
        // 添加符号表
        global.currentEnv->getParent()->func_table.setInfo(nameId, info);


        // 处理函数体
        auto body = ctx->block();
        funcDeclNode->entry_node = std::any_cast<HNode>(body->accept(this));

        global.tree->setupEnv(funcDeclNode, global.currentEnv);
        global.currentEnv = global.tree->popEnv();
        // 添加节点
        global.tree->setNode(funcDeclId, funcDeclNode);
        return funcDeclId;
    }

    // 形参列表声明
    std::any SyntaxTreeBuilder::Visitor::visitFuncFParams(SysYParser::FuncFParamsContext *ctx) {
        auto param = ctx->funcFParam();
        auto funcParamNode = param
        | views::transform([&](auto i) {
            return std::any_cast<HNode>(i->accept(this));
        });
        return std::vector<HNode>{funcParamNode.begin(), funcParamNode.end()};
    }

    // 形参
    std::any SyntaxTreeBuilder::Visitor::visitFuncFParam(SysYParser::FuncFParamContext *ctx) {
        auto &baseType = toType(ctx->type()->getText());

        // 构建类型
        auto isPointer = ctx->subscript != nullptr;
        const Type *type;
        if (!isPointer) {
            type = &baseType;
        } else {
            auto innerArrInd = ctx->exp();
            if (innerArrInd.empty()) {
                type = &Pointer::create(baseType);
            } else {
                auto innerArrNum = innerArrInd
                | views::transform([&](auto i) {
                    return std::any_cast<HNode>(i->accept(this));
                })
                | views::reverse
                | views::transform([&](auto i) {
                    return std::get<std::int32_t>(eval.compute(i));
                });
                auto &arr = Array::create(baseType, {innerArrNum.begin(), innerArrNum.end()});
                type = &Pointer::create(arr);
            }
        };

        VarInfo info;
        info.name = ctx->Ident()->getText();
        info.type = type;
        info.isConstant = false;
        info.isParam = true;
        auto nodeId = global.tree->pushNode();
        auto node = new ParamDecl;
        node->info_id = global.currentEnv->getId(info.name);
        info.decl = nodeId;
        global.currentEnv->var_table.setInfo(node->info_id, info);
        global.tree->setNode(nodeId, node);
        return nodeId;
    }

    // 语句
    std::any SyntaxTreeBuilder::Visitor::visitBlock(SysYParser::BlockContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto nodeId = global.tree->pushNode();
        global.currentEnv = global.tree->pushEnv();
        std::vector<HNode> subNodes{};
        for (auto i : ctx->blockItem()) {
            auto result = i->accept(this);
            if (result.type() == typeid(HNode)) {
                subNodes.push_back(std::any_cast<HNode>(result));
            } else {
                for (auto d : std::any_cast<std::vector<HNode>>(result)) {
                    subNodes.push_back(d);
                }
            }
        }
        auto node = new Block(subNodes);
        global.tree->setNode(nodeId, node);
        global.tree->setupEnv(node, global.currentEnv);
        global.currentEnv = global.tree->popEnv();
        return nodeId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitAssign(SysYParser::AssignContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto nodeId = global.tree->pushNode();
        auto lhsCtx = ctx->lVal();
        auto rhsCtx = ctx->exp();
        auto lhsId = std::any_cast<HNode>(lhsCtx->accept(this));
        auto rhsId = std::any_cast<HNode>(rhsCtx->accept(this));
        auto lhsType = global.tree->getNode<Expr>(lhsId)->type;
        rhsId = convertTo(*lhsType, rhsId);
        auto node = new Assign(lhsId, rhsId);
        global.tree->setNode(nodeId, node);
        return nodeId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitIf(SysYParser::IfContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto nodeId = global.tree->pushNode();
        auto condId = std::any_cast<HNode>(ctx->cond()->accept(this));
        condId = convertToCond(condId);
        auto stmt = std::any_cast<HNode>(ctx->stmt(0)->accept(this));
        HNode else_stmt = -1;
        if (ctx->stmt(1)) {
            else_stmt = std::any_cast<HNode>(ctx->stmt(1)->accept(this));
        }
        auto node = new If(condId, stmt, else_stmt);
        global.tree->setNode(nodeId, node);
        return nodeId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitWhile(SysYParser::WhileContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto nodeId = global.tree->pushNode();
        auto condId = std::any_cast<HNode>(ctx->cond()->accept(this));
        auto bodyId = std::any_cast<HNode>(ctx->stmt()->accept(this));
        auto node = new While(condId, bodyId);
        global.tree->setNode(nodeId, node);
        return nodeId;
    }

    std::any SyntaxTreeBuilder::Visitor::visitContinue(SysYParser::ContinueContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        return global.tree->pushNode(new Continue);
    }

    std::any SyntaxTreeBuilder::Visitor::visitBreak(SysYParser::BreakContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        return global.tree->pushNode(new Break);
    }

    std::any SyntaxTreeBuilder::Visitor::visitReturn(SysYParser::ReturnContext *ctx) {
        LOG_TRACE("To process source line {}", ctx->getStart()->getLine());
        auto nodeId = global.tree->pushNode();
        auto returnedCtx = ctx->exp();
        if (returnedCtx) {
            auto expId = std::any_cast<HNode>(ctx->exp()->accept(this));
            expId = convertTo(*_returned_type, expId);
            auto node = new Return(expId);
            global.tree->setNode(nodeId, node);
            return nodeId;
        } else {
            auto returnedNode = global.tree->pushNode(new Empty);
            return returnedNode;
        }
    }

} // AST