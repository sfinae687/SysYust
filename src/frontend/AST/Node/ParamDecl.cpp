//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/ParamDecl.h"

#include "AST/Node/NodeExecutor.h"


namespace SysYust::AST {
    void ParamDecl::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST