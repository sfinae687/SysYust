//
// Created by LL06p on 24-7-6.
//

#ifndef SYSYUST_AST_SYNTAXTREEBUILDER_H
#define SYSYUST_AST_SYNTAXTREEBUILDER_H

#include <SysYBaseVisitor.h>
#include <SysYParser.h>

#include "SyntaxTree.h"

namespace SysYust::AST {

    using ParserTree = antlr4::tree::ParseTree;

    /**
     * @brief 实现语法树构建功能的类,不可由用户构造
     */
    class SyntaxTreeBuilder {
    public:

        std::unique_ptr<SyntaxTree> getTree();

        static std::unique_ptr<SyntaxTree> getTree(SysYParser::CompUnitContext* tree);

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

            std::any visitCompUnit(SysYParser::CompUnitContext *ctx) override;
            std::any visitConstDecl(SysYParser::ConstDeclContext *ctx) override;

            // 复制工具

            static const Type& toType(std::string_view type);

        private:
            SyntaxTreeBuilder &global;
        };

        Visitor v;
    };


} // AST

#endif //SYSYUST_AST_SYNTAXTREEBUILDER_H
