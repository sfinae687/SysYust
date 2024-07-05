//
// Created by LL06p on 24-7-5.
//

#include "AST/Not.h"
#include "AST/NodeExecutor.h"


namespace SysYust::AST {
    Not::Not(std::size_t e)
    : CondExpr()
    , subexpr(e) {

    }

    void Not::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST