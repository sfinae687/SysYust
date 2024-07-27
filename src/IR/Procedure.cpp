//
// Created by LL06p on 24-7-20.
//

#include <utility>

#include "IR/Procedure.h"


namespace SysYust::IR {
    Procedure::Procedure(CodeContext &code, func_symbol name, std::vector<var_symbol> params)
        : _context(code)
        , _name(std::move(name))
    {
        context()->param_var_set = std::move(params);
    }

    Procedure::Procedure(CodeContext &code, func_symbol name, func_info info,
                         std::vector<var_symbol> params)
        : Procedure(code, name, params)
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

    func_symbol Procedure::name() const {
        return _name;
    }

    bool Procedure::is_param(var_symbol name) {
        return _context.is_param(name);
    }

    bool Procedure::remove_param(var_symbol name) {
        auto founded = std::find(context()->param_var_set.begin(), context()->param_var_set.end(), name);
        if (founded != context()->param_var_set.end()) {
            context()->param_var_set.erase(founded);
            return true;
        } else {
            return false;
        }
    }

    void Procedure::add_param(var_symbol name) {
        _context.param_var_set.push_back(name);
    }

    std::size_t Procedure::param_index(var_symbol name) {
        return _context.param_index(name);
    }

} // IR