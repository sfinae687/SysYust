//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_CALL_H
#define SYSYUST_AST_CALL_H

#include <utility>
#include <vector>

#include "NodeBase.h"
#include "Expr.h"
#include "IdAllocator.h"

namespace SysYust::AST {

    class Call : public Expr {
    public:
        Call(const Type *returnType, NumId id, std::vector<std::size_t> args);
        NumId func_info;
        std::vector<std::size_t> argumentExpr;

        void execute(NodeExecutor *e) override;

    };

} // AST

#endif //SYSYUST_AST_CALL_H
