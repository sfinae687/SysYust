//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/NodeExecutor.h"
#include "AST/Node/Return.h"

namespace SysYust::AST {
    Return::Return(HNode returned)
    : Stmt()
    , returned(returned) {

    }

    void Return::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST