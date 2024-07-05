//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/ArrayRef.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust::AST {
    ArrayRef::ArrayRef(Type *type, NumId id, std::vector<std::size_t> sub)
    : LExpr(type, false)
    , var_id(id)
    , subscripts(std::move(sub)) {

    }

    void ArrayRef::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST