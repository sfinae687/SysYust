//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_UNARYOP_H
#define SYSYUST_AST_UNARYOP_H

#include "Expr.h"

namespace SysYust::AST {

    class UnaryOp : public Expr {
    public:
        enum OpType {Positive, Negative};

        UnaryOp(Type *e_type, OpType t, std::size_t s);
        OpType type;
        std::size_t subexpr;

        void execute(NodeExecutor *e) override;
    };

} // AST

#endif //SYSYUST_AST_UNARYOP_H
