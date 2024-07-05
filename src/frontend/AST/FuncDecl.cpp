//
// Created by LL06p on 24-7-5.
//

#include "AST/FuncDecl.h"
#include "AST/NodeExecutor.h"

namespace SysYust::AST {
    void FuncDecl::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST