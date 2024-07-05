//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/Not.h"
#include "AST/Node/NodeExecutor.h"


namespace SysYust::AST {
    Not::Not(std::size_t e)
    : CondExpr()
    , subexpr(e) {

    }

    void Not::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST