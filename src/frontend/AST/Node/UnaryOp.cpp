//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/UnaryOp.h"

namespace SysYust::AST {
    UnaryOp::UnaryOp(const Type *e_type, OpType t, HNode s)
    : Expr(e_type, false)
    , type(t)
    , subexpr(s) {

    }

    void UnaryOp::execute(NodeExecutor *e) {

    }
} // AST