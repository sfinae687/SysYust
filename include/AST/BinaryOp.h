//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_BINARYOP_H
#define SYSYUST_AST_BINARYOP_H

#include "Expr.h"

namespace SysYust::AST {

    class BinaryOp : public Expr {
    public:
        enum OpType {Add, Sub, Mul, Div, Mod, };

        BinaryOp(Type *e_type, OpType opt, std::size_t lhs, std::size_t rhs);

        OpType type;
        std::size_t lhs;
        std::size_t rhs;

        void execute(NodeExecutor *e) override;
    };

} // AST

#endif //SYSYUST_AST_BINARYOP_H
