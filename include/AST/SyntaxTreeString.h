//
// Created by LL06p on 24-7-12.
//

#ifndef SYSYUST_AST_SYNTAXTREESTRING_H
#define SYSYUST_AST_SYNTAXTREESTRING_H

#include <string>
#include <vector>

#include "AST/Node.h"
#include "SyntaxTree.h"

namespace SysYust::AST {

    class SyntaxTreeString : NodeExecutorBase {
    public:
        explicit SyntaxTreeString(const SyntaxTree &tree);
        explicit operator std::string () ;

        void execute(const VarDecl &decl) override;

        void execute(const FuncDecl &decl) override;

        void execute(const ParamDecl &decl) override;

        void execute(const IntLiteral &literal) override;

        void execute(const FloatLiteral &literal) override;

        void execute(const Call &call) override;

        void execute(const UnaryOp &op) override;

        void execute(const BinaryOp &op) override;

        void execute(const DeclRef &decl) override;

        void execute(const ArrayRef &array) override;

        void execute(const ToInt &anInt) override;

        void execute(const ToFloat &aFloat) override;

        void execute(const Not &aNot) override;

        void execute(const And &anAnd) override;

        void execute(const Or &anOr) override;

        void execute(const Compare &compare) override;

        void execute(const ToCond &cond) override;

        void execute(const If &anIf) override;

        void execute(const Assign &assign) override;

        void execute(const While &aWhile) override;

        void execute(const Break &aBreak) override;

        void execute(const Continue &aContinue) override;

        void execute(const Return &aReturn) override;

        void execute(const Block &block) override;

        void execute(const Empty &empty) override;

        void execute(const List &list) override;

    private:
        std::pair<std::string, std::string> executeTwo(HNode lhs, HNode rhs);
        std::vector<std::string> strings;
        const SyntaxTree &tree;
        std::shared_ptr<Env> currentEnv = tree.topEnv();
    };

} // AST

#endif //SYSYUST_AST_SYNTAXTREESTRING_H
