//
// Created by LL06p on 24-7-5.
//

#include "AST/VarDecl.h"
#include "AST/NodeExecutor.h"

namespace SysYust::AST {
    void VarDecl::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST