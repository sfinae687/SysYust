//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_OR_H
#define SYSYUST_AST_OR_H

#include <utility>

#include "CondExpr.h"

namespace SysYust::AST {

    class Or : public CondExpr {
    public:
        Or(std::size_t lhs, std::size_t rhs);

        void execute(NodeExecutor *e) override;

        std::size_t lhs;
        std::size_t rhs;
    };

} // AST

#endif //SYSYUST_AST_OR_H
