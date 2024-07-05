//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/NodeExecutor.h"
#include "AST/Node/Or.h"

namespace SysYust::AST {
    Or::Or(std::size_t lhs, std::size_t rhs)
    : CondExpr()
    , lhs(lhs)
    , rhs(rhs) {

    }

    void Or::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST