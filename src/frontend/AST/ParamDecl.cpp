//
// Created by LL06p on 24-7-5.
//

#include "AST/ParamDecl.h"

#include "AST/NodeExecutor.h"


namespace SysYust::AST {
    void ParamDecl::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST