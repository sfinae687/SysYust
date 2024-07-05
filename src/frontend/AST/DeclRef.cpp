//
// Created by LL06p on 24-7-5.
//

#include <cassert>

#include "AST/DeclRef.h"
#include "AST/NodeExecutor.h"

namespace SysYust::AST {
    DeclRef::DeclRef(Type *e_type, NumId id)
    : LExpr(e_type, false)
    , var_id(id) {
        assert(type == &Int_v || type == &Float_v);
    }

    void DeclRef::execute(NodeExecutor *e) {
        e->execute(*this);
    }
} // AST