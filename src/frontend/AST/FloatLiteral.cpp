//
// Created by LL06p on 24-7-5.
//

#include "AST/FloatLiteral.h"
#include "AST/NodeExecutor.h"

namespace SysYust::AST {
    FloatLiteral::FloatLiteral()
    : Expr(&Float_v, true) {

    }

    FloatLiteral::FloatLiteral(float v)
    : Expr(&Float_v, true)
    , value(v) {

    }

    void FloatLiteral::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST