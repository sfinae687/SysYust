//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_TOFLOAT_H
#define SYSYUST_AST_TOFLOAT_H

#include "Expr.h"

namespace SysYust::AST {

    class ToFloat : public Expr {
    public:
        explicit ToFloat(std::size_t operant);

        void execute(NodeExecutor *e) override;

        std::size_t operant;
    };

} // AST

#endif //SYSYUST_AST_TOFLOAT_H
