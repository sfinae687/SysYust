//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/ToFloat.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust::AST {
    void ToFloat::execute(NodeExecutor *e) {
        e->execute(*this);
    }

    ToFloat::ToFloat(std::size_t operant)
    : Expr(&Float_v, false)
    , operant(operant) {

    }
} // AST