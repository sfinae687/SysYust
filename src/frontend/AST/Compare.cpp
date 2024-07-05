//
// Created by LL06p on 24-7-5.
//

#include "AST/Compare.h"
#include "AST/NodeExecutor.h"

namespace SysYust::AST {
    Compare::Compare(Compare::CompareType type, std::size_t lhs, std::size_t rhs)
    : type(type)
    , lhs(lhs)
    , rhs(rhs) {

    }

    void Compare::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST