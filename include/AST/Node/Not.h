//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_NOT_H
#define SYSYUST_AST_NOT_H

#include <utility>

#include "Expr.h"

namespace SysYust::AST {

    class Not : public Expr {
    public:
        explicit Not(HNode e);

        void execute(NodeExecutor *e) override;

        std::size_t subexpr;
    };

} // AST

#endif //SYSYUST_AST_NOT_H
