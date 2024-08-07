//
// Created by LL06p on 24-7-26.
//

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

#include "IR/ContextualMixin.h"

namespace SysYust::IR {

    namespace ranges = ::ranges;
    namespace views = ranges::views;

    ContextualMixin::ContextualMixin(Code *code, Procedure *proc)
        : _ir_code(code)
        , _code_context(&code->context)
        , _current_procedure(proc)
        , _procedure_context(proc ? proc->context() : nullptr)
    {

    }

    void ContextualMixin::set_context(Code *code, Procedure *proc) {
        if (code) {
            _ir_code = code;
            _code_context = &code->context;
            if (proc) {
                _current_procedure = proc;
                _procedure_context = proc->context();
            } else {
                _current_procedure = nullptr;
                _procedure_context = nullptr;
            }
        } else {
            clear_context();
        }
    }

    void ContextualMixin::clear_context() {
        _ir_code = nullptr;
        _code_context = nullptr;
        _current_procedure = nullptr;
        _procedure_context = nullptr;
        while (!_block_stack.empty()) {
            _block_stack.pop();
        }
    }

    void ContextualMixin::set_procedure(Procedure *proc) {
        _current_procedure = proc;
        _procedure_context = proc ? proc->context() : nullptr;
    }

    BasicBlock *ContextualMixin::current_basic_block() const {
        if (_block_stack.empty()) {
            return nullptr;
        }
        return _block_stack.top();
    }

    void ContextualMixin::set_basic_block(BasicBlock *blk) {
        if (_block_stack.empty()) {
            _block_stack.push(blk);
        } else {
            _block_stack.top() = blk;
        }
    }

    void ContextualMixin::next_basic_block() {
        if (current_basic_block() && current_basic_block()->nextBlock()) {
            set_basic_block(current_basic_block()->nextBlock());
        }
    }

    void ContextualMixin::else_basic_block() {
        if (current_basic_block() && current_basic_block()->elseBlock()) {
            set_basic_block(current_basic_block()->elseBlock());
        }
    }

    void ContextualMixin::save_basic_block() {
        if (_block_stack.empty() || !_block_stack.top()) {

        } else {
            _block_stack.push(current_basic_block());
        }
    }

    void ContextualMixin::save_and_set(BasicBlock *blk) {
        save_basic_block();
        set_basic_block(blk);
    }

    void ContextualMixin::save_and_next() {
        save_basic_block();
        next_basic_block();
    }

    void ContextualMixin::save_and_else() {
        save_basic_block();
        else_basic_block();
    }

    void ContextualMixin::pop_basic_block() {
        if (!_block_stack.empty()) {
            _block_stack.pop();
        }
    }

    Code *ContextualMixin::ir_code() const {
        return _ir_code;
    }

    CodeContext *ContextualMixin::global_context() const {
        return _code_context;
    }

    Procedure *ContextualMixin::procedure() const {
        return _current_procedure;
    }

    ProcedureContext *ContextualMixin::procedure_context() {
        return _procedure_context;
    }

    ControlFlow *ContextualMixin::control_flow() const {
        return _current_procedure ? &_current_procedure->getGraph() : nullptr;
    }

    Procedure *ContextualMixin::seekProcedure(func_symbol sym) const {
        auto &list = _ir_code->procedures();
        auto founded = ranges::find(list.begin(), list.end(), sym, &Procedure::name);
        if (founded != list.end()) {
            return &*founded;
        } else {
            return nullptr;
        }
    }
}