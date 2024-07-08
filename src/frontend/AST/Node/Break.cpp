//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/NodeExecutor.h"
#include "AST/Node/Break.h"

namespace SysYust::AST {
    void Break::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST