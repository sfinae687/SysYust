//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_TOCOND_H
#define SYSYUST_AST_TOCOND_H

#include <utility>

#include "CondExpr.h"

namespace SysYust::AST {

    class ToCond : public CondExpr {
    public:
        explicit ToCond(HNode opr);

        void execute(NodeExecutor *e) override;

        std::size_t operant;
    };

} // AST

#endif //SYSYUST_AST_TOCOND_H
