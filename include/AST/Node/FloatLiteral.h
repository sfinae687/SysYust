//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_FLOATLITERAL_H
#define SYSYUST_AST_FLOATLITERAL_H

#include "Expr.h"

namespace SysYust::AST {

    class FloatLiteral : public Expr {
    public:
        FloatLiteral();
        explicit FloatLiteral(float v);
        float value = 0.f;

        void execute(NodeExecutor *e) override;
    };

} // AST

#endif //SYSYUST_AST_FLOATLITERAL_H
