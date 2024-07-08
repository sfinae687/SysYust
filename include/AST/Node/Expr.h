//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_EXPR_H
#define SYSYUST_AST_EXPR_H

#include "Stmt.h"
#include "AST/Type.h"

namespace SysYust::AST {

    class Expr : public Stmt {
    public:
        explicit Expr(const Type *type, bool isConst = false);
        const Type *type; ///< 当 type 为 nullptr 时标识当前表达式类型待决
        bool isConsteval = false;
    };

} // AST

#endif //SYSYUST_AST_EXPR_H
