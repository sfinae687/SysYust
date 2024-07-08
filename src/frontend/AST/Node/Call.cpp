//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/Call.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust::AST {
    Call::Call(const Type *returnType, NumId id, std::vector<HNode> args)
    : Expr(returnType)
    , func_info(id)
    , argumentExpr(std::move(args)) {

    }

    void Call::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST