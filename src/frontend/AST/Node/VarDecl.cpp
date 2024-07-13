//
// Created by LL06p on 24-7-5.
//

#include <format>
#include "AST/Node/VarDecl.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust::AST {
    void VarDecl::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST