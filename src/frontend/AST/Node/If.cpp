//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/NodeExecutor.h"
#include "AST/Node/If.h"

namespace SysYust::AST {
    If::If(std::size_t cond, std::size_t stmts)
    : Stmt()
    , cond(cond)
    , stmts(stmts) {

    }

    void If::execute(NodeExecutor *e) {
        e->execute(*this);
    }

    If::If(std::size_t cond, HNode stmts, HNode else_stmts)
    : Stmt()
    , cond(cond)
    , stmts(stmts)
    , else_stmt(else_stmts) {

    }
} // AST