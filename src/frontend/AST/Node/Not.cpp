//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/Not.h"
#include "AST/Node/NodeExecutor.h"


namespace SysYust::AST {
    Not::Not(HNode e)
        : Expr(&Int_v, false)
        , subexpr(e)
        {

    }

    void Not::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST