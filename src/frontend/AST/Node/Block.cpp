//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/NodeExecutor.h"
#include "AST/Node/Block.h"

namespace SysYust::AST {
    Block::Block(std::vector<std::size_t> stmts)
    : Stmt()
    , stmts(std::move(stmts)) {

    }

    void Block::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST