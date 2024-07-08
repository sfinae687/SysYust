//
// Created by LL06p on 24-7-4.
//

#include "AST/Env.h"

namespace SysYust::AST {
    Env::Env(std::shared_ptr<Env> parent)
    : _parent(std::move(parent))
    , var_table(&parent->var_table)
    , func_table(&parent->func_table)
    , _name_id(parent->_name_id) {

    }

    NumId Env::getId(const Env::name_t& name) {
        return _name_id->idFor(name);
    }

    std::shared_ptr<Env> Env::getParent() const {
        return _parent;
    }
} // AST