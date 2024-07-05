//
// Created by LL06p on 24-7-5.
//

#include "AST/NodeExecutor.h"
#include "AST/Assign.h"

namespace SysYust::AST {
    Assign::Assign(std::size_t l_val, std::size_t r_val)
    : Stmt()
    , l_val(l_val)
    , r_val(r_val) {

    }

    void Assign::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST