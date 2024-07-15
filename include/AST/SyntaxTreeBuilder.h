//
// Created by LL06p on 24-7-6.
//

#ifndef SYSYUST_AST_SYNTAXTREEBUILDER_H
#define SYSYUST_AST_SYNTAXTREEBUILDER_H

#include "SysYBaseVisitor.h"
#include "SysYParser.h"

#include "SyntaxTree.h"
#include "ConstantEvaluator.h"

namespace SysYust::AST {

    using ParserTree = antlr4::tree::ParseTree;

    /**
     * @brief 实现语法树构建功能的类,不可由用户构造
     */
    class SyntaxTreeBuilder {
    public:

        std::unique_ptr<SyntaxTree> getTree();

        static std::unique_ptr<SyntaxTree> getTree(SysYParser::CompUnitContext* tree);

        static std::set<FuncInfo> lib_funcs;
        static std::set<NumId> lib_funcs_id;
    private:
        explicit SyntaxTreeBuilder(SysYParser::CompUnitContext* tree);

        SysYParser::CompUnitContext* rawTree = nullptr;
        std::unique_ptr<SyntaxTree> tree = std::make_unique<SyntaxTree>();
        std::shared_ptr<Env> currentEnv = tree->topEnv();

        friend class Visitor;
        class Visitor : public SysYBaseVisitor {
        public:
            explicit Visitor(SyntaxTreeBuilder &);

            // 结构

            // 整个翻译单元
            std::any visitCompUnit(SysYParser::CompUnitContext *ctx) override;
            // 常量声明
            std::any visitConstDecl(SysYParser::ConstDeclContext *ctx) override;
            // 变量声明
            std::any visitVarDecl(SysYParser::VarDeclContext *ctx) override;

            // 列表，返回列表节点的编号
            std::any visitConstListInit(SysYParser::ConstListInitContext *ctx) override;
            std::any visitListInit(SysYParser::ListInitContext *ctx) override;

            // 表达式, 返回节点编号

            // 直接委托给子节点的生成式
            /// @todo 突然发现 BaseVisitor 的默认实现就是委托给子节点，这些似乎可以删除
            /*Add*/ std::any visitFromMul(SysYParser::FromMulContext *ctx) override;
            /*Mul*/ std::any visitFromUnary(SysYParser::FromUnaryContext *ctx) override;
            /*Unary*/ std::any visitFromPrimary(SysYParser::FromPrimaryContext *ctx) override;
            /*Primary*/ std::any visitClosedExpr(SysYParser::ClosedExprContext *ctx) override;
            /*Primary*/ std::any visitLValValue(SysYParser::LValValueContext *ctx) override;
            /*Primary*/ std::any visitLiteralValue(SysYParser::LiteralValueContext *ctx) override;
            /*Stmt*/ std::any visitExpr(SysYParser::ExprContext *ctx) override;

            // 使用子节点的生成式
            /*Add*/ std::any visitAddOp(SysYParser::AddOpContext *ctx) override;
            /*Mul*/ std::any visitMulOp(SysYParser::MulOpContext *ctx) override;
            /*unary*/ std::any visitCall(SysYParser::CallContext *ctx) override;
            /*unary*/ std::any visitOpUnary(SysYParser::OpUnaryContext *ctx) override;
            /*Call*/ HNode processTimeLib(SysYParser::CallContext *ctx);

            // 基本表达式
            /*basic*/ std::any visitLVal(SysYParser::LValContext *ctx) override;
            /*basic*/ std::any visitFloatNumber(SysYParser::FloatNumberContext *ctx) override;
            /*basic*/ std::any visitIntNumber(SysYParser::IntNumberContext *ctx) override;

            // 条件表达式
            /*relExp*/ std::any visitRelOp(SysYParser::RelOpContext *ctx) override;
            /*eqExp*/ std::any visitEqOp(SysYParser::EqOpContext *ctx) override;
            /*lAndExp*/ std::any visitLAndOp(SysYParser::LAndOpContext *ctx) override;
            /*lOrExp*/ std::any visitLOrOp(SysYParser::LOrOpContext *ctx) override;

            // 函数声明
            std::any visitFuncDef(SysYParser::FuncDefContext *ctx) override;
            std::any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override;
            std::any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override;

            // 语句
            std::any visitBlock(SysYParser::BlockContext *ctx) override;
            std::any visitAssign(SysYParser::AssignContext *ctx) override;
            std::any visitIf(SysYParser::IfContext *ctx) override;
            std::any visitWhile(SysYParser::WhileContext *ctx) override;
            std::any visitContinue(SysYParser::ContinueContext *ctx) override;
            std::any visitBreak(SysYParser::BreakContext *ctx) override;
            std::any visitReturn(SysYParser::ReturnContext *ctx) override;

            // 辅助工具
            static const Type& toType(std::string_view type);
            /**
             * @brief 表达式的隐式类型转换
             */
            std::pair<HNode, HNode> numberTypeCast(HNode lhs, HNode rhs);
            HNode convertTo(const Type &t, HNode n);
            HNode convertToCond(HNode n);

            /// @brief 库函数表
        private:
            SyntaxTreeBuilder &global;
            ConstantEvaluator eval;
        };

        Visitor v;
    };


} // AST

#endif //SYSYUST_AST_SYNTAXTREEBUILDER_H
