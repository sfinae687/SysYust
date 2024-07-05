//
// Created by LL06p on 24-7-5.
//

#include "AST/NodeExecutor.h"
#include "AST/ToCond.h"

namespace SysYust::AST {
    ToCond::ToCond(std::size_t opr)
    : CondExpr()
    , operant(opr) {

    }

    void ToCond::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST