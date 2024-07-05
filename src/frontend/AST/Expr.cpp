//
// Created by LL06p on 24-7-5.
//

#include "AST/Expr.h"

namespace SysYust::AST {
    Expr::Expr(const Type *type, bool isConst)
    : type(type)
    , isConsteval(isConst) {

    }
} // AST