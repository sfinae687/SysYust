//
// Created by LL06p on 24-7-10.
//

#ifndef SYSYUST_CONSTANTEVALUATOR_H
#define SYSYUST_CONSTANTEVALUATOR_H

#include <vector>
#include <stack>
#include <variant>

#include "AST/Node.h"
#include "AST/SyntaxTree.h"

namespace SysYust::AST {

    class ConstantEvaluator : NodeExecutorBase {
    public:
        using value_t = std::variant<std::int32_t, float>;
        explicit ConstantEvaluator(const SyntaxTree &tree);

        value_t compute(HNode exp);

        void execute(const SysYust::AST::BinaryOp &) override;
        void execute(const SysYust::AST::DeclRef &) override;
        void execute(const SysYust::AST::ToFloat &) override;
        void execute(const SysYust::AST::UnaryOp &) override;
        void execute(const SysYust::AST::ArrayRef &) override;
        void execute(const SysYust::AST::List &) override;
        void execute(const SysYust::AST::FloatLiteral &) override;
        void execute(const SysYust::AST::IntLiteral &) override;
    private:
        std::stack<value_t> valueStack = {};
        const SyntaxTree &tree;
    };

} // AST

#endif //SYSYUST_CONSTANTEVALUATOR_H
