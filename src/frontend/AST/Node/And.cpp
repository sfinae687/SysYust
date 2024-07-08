//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/And.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust::AST {
    void And::execute(NodeExecutor *e) {
        e->execute(*this);
    }

    And::And(HNode lhs, HNode rhs)
    : CondExpr()
    , lhs(lhs)
    , rhs(rhs) {

    }
} // AST