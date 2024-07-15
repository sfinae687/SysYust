//
// Created by LL06p on 24-7-11.
//

#include "AST/Node/NodeExecutor.h"

#include "AST/Node/List.h"

namespace SysYust::AST {
    List::List(const std::vector<HNode> &vals)
    : vals(vals.begin(), vals.end()) {

    }

    void List::execute(SysYust::AST::NodeExecutor *e) {
        e->execute(*this);
    }
} // AST