//
// Created by LL06p on 24-7-5.
//

#include "AST/NodeExecutor.h"
#include "AST/Return.h"

namespace SysYust::AST {
    Return::Return(std::size_t returned)
    : Stmt()
    , returned(returned) {

    }

    void Return::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST