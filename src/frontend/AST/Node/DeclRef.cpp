//
// Created by LL06p on 24-7-5.
//

#include <cassert>

#include "AST/Node/DeclRef.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust::AST {
    DeclRef::DeclRef(const Type *e_type, NumId id)
    : LExpr(e_type, false)
    , var_id(id) {

    }

    void DeclRef::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST