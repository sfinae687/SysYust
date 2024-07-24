//
// Created by LL06p on 24-7-20.
//

#include <utility>

#include "IR/Procedure.h"


namespace SysYust::IR {
    Procedure::Procedure(CodeContext &code, func_symbol name)
        : _context(code)
        , _name(std::move(name))
    {

    }

    Procedure::Procedure(CodeContext &code, func_symbol name, func_info info)
        : Procedure(code, name)
    {
        code.set_func(name, info);
    }

    ControlFlow &Procedure::getGraph() {
        return _graph;
    }

    func_info Procedure::info() const {
        return _context.global.get_func(_name);
    }

    const Type *Procedure::type() const {
        return _context.global.get_fun_type(_name);
    }

    ProcedureContext *Procedure::context() {
        return &_context;
    }

    const ProcedureContext *Procedure::context() const {
        return &_context;
    }

} // IR