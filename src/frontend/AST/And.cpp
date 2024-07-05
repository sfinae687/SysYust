//
// Created by LL06p on 24-7-5.
//

#include "AST/And.h"
#include "AST/NodeExecutor.h"

namespace SysYust::AST {
    void And::execute(NodeExecutor *e) {
        e->execute(*this);
    }

    And::And(std::size_t lhs, std::size_t rhs)
    : CondExpr()
    , lhs(lhs)
    , rhs(rhs) {

    }
} // AST