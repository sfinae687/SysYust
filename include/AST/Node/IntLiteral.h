//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_INTLITERAL_H
#define SYSYUST_AST_INTLITERAL_H

#include <utility>
#include <cstdint>

#include "Expr.h"

namespace SysYust::AST {

    class IntLiteral : public Expr {
    public:
        IntLiteral();
        explicit IntLiteral(std::int32_t v);
        std::int32_t value = 0;

        void execute(NodeExecutor *e) override;
    };

} // AST

#endif //SYSYUST_AST_INTLITERAL_H
