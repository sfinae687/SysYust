//
// Created by LL06p on 24-7-24.
//

#include "IR/CodeUtil.h"

namespace SysYust::IR {

    void CodeBuildMixin::reset() {
        delete ir_code;
        _code_context = nullptr;
        _current_procedure = nullptr;
        _procedure_context = nullptr;
        while (!_current_block.empty()) {
            _current_block.pop();
        }
    }

    void CodeBuildMixin::setup_code() {
        reset();
        ir_code = new Code;
        _code_context = &ir_code->context;
    }

    void CodeBuildMixin::setGlobalVar(const var_symbol& sym) {
        _code_context->addGlobal(sym);
    }

    void CodeBuildMixin::entry_function(func_symbol name, func_info info) {

    }

    Procedure *CodeBuildMixin::current_procedure() {
        return nullptr;
    }

    Procedure *CodeBuildMixin::procedure(func_symbol name) {
        return nullptr;
    }

    void CodeBuildMixin::exit_function() {

    }

    BasicBlock *CodeBuildMixin::newBasicBlock() {
        return nullptr;
    }

    void CodeBuildMixin::entry_block() {

    }

    void CodeBuildMixin::entry_block(BasicBlock *blk) {

    }

    void CodeBuildMixin::entry_next() {

    }

    void CodeBuildMixin::entry_else() {

    }

    void CodeBuildMixin::push_block() {

    }

    void CodeBuildMixin::push_block(BasicBlock *blk) {

    }

    void CodeBuildMixin::pop_back() {

    }

    BasicBlock *CodeBuildMixin::current_block() const {
        return nullptr;
    }

    void CodeBuildMixin::setTarget(BasicBlock *blk) {

    }

    void CodeBuildMixin::setElseTarget(BasicBlock *blk) {

    }

    bool CodeBuildMixin::is_global(const var_symbol &sym) {
        return _code_context->is_global(sym);
    }

    Code *CodeBuildMixin::getCode() {
        auto rt = ir_code;
        ir_code = nullptr;
        reset();
        return rt;
    }

}