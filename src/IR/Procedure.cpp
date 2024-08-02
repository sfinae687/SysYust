//
// Created by LL06p on 24-7-20.
//

#include <utility>

#include "IR/Procedure.h"


namespace SysYust::IR {
    Procedure::Procedure(CodeContext &code, func_symbol name, std::vector<var_symbol> params)
        : _context(code)
        , _name(std::move(name))
        , _du(*this)
    {
        context()->param_var_set = std::move(params);
        resync_param();
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
            _du.remove_define(*founded);
            return true;
        } else {
            return false;
        }
    }

    void Procedure::add_param(var_symbol name) {
        _du.add_func_param(name);
        _context.param_var_set.push_back(name);
    }

    std::size_t Procedure::param_index(var_symbol name) {
        return _context.param_index(name);
    }

    ProcedureDU & Procedure::du_info() {
        return _du;
    }

    const ProcedureDU &Procedure::du_info() const {
        return _du;
    }

    void Procedure::retrack_symbol_as(IR::value_type sym, IR::value_type nSym) {
        _du.rename_define(sym, nSym);
    }

    void Procedure::untrack_symbol(IR::value_type sym) {
        _du.remove_define(sym);
    }

    void Procedure::track_block_arg(BasicBlock *block, var_symbol arg) {
        _du.add_block_param(block, arg);
    }

    void Procedure::retrack_inst(BasicBlock::iterator inst_ptr, const instruction &nInst) {
        _du.set_inst(inst_ptr, nInst);
    }

    void Procedure::untrack_inst(BasicBlock::iterator inst_ptr) {
        _du.remove_inst(inst_ptr);
    }

    void Procedure::track_inst(BasicBlock *block, BasicBlock::iterator inst_ptr) {
        _du.add_inst(block, inst_ptr);
    }

    void Procedure::clear_param() {
        for (auto &i : context()->param_var_set) {
            _du.remove_define(i);
        }
        context()->param_var_set.clear();
    }

    void Procedure::resync_param() {
        for (auto &i : context()->param_var_set) {
            _du.add_func_param(i);
        }
    }

} // IR