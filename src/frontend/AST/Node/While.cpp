//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/NodeExecutor.h"
#include "AST/Node/While.h"

namespace SysYust::AST {
    While::While(HNode cond, HNode stmt)
    : Stmt()
    , cond(cond)
    , stmt(stmt) {

    }

    void While::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST