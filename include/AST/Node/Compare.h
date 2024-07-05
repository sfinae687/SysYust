//
// Created by LL06p on 24-7-5.
//


#ifndef SYSYUST_AST_COMPARE_H
#define SYSYUST_AST_COMPARE_H

#include <utility>

#include "CondExpr.h"

namespace SysYust::AST {

    class Compare : public CondExpr {
    public:
        enum CompareType {EQ, NE, GT, GE, LT, LE};

        Compare(CompareType type, std::size_t lhs, std::size_t rhs);

        void execute(NodeExecutor *e) override;

        CompareType type;
        std::size_t lhs;
        std::size_t rhs;
    };

} // AST

#endif //SYSYUST_AST_COMPARE_H
