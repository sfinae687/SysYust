//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_TOINT_H
#define SYSYUST_AST_TOINT_H

#include "Expr.h"

namespace SysYust::AST {

    class ToInt : public Expr {
    public:
        explicit ToInt(HNode operant);

        void execute(NodeExecutor *e) override;

        std::size_t operant;
    };

} // AST

#endif //SYSYUST_AST_TOINT_H
