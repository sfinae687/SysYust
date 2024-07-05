//
// Created by LL06p on 24-7-5.
//

#include "AST/NodeExecutor.h"
#include "AST/Break.h"

namespace SysYust::AST {
    void Break::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST