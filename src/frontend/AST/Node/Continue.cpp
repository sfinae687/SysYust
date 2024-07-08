//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/NodeExecutor.h"
#include "AST/Node/Continue.h"

namespace SysYust::AST {
    void Continue::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST