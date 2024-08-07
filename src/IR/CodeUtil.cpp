//
// Created by LL06p on 24-7-24.
//

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

#include "IR/CodeUtil.h"
#include "IR/ContextualMixin.h"

namespace SysYust::IR {

    namespace ranges = ::ranges;
    namespace views = ranges::views;

    // IR 级

    void CodeBuildMixin::reset() {
        delete _ir_code;
        clear_context();
    }

    void CodeBuildMixin::setup_code() {
        reset();
        set_context(new Code);
    }

    void CodeBuildMixin::setGlobalVar(const var_symbol& sym) {
        global_context()->addGlobal(sym);
    }

    Code *CodeBuildMixin::getCode() {
        auto rt = ir_code();
        clear_context();
        return rt;
    }

    bool CodeBuildMixin::is_global(const var_symbol &sym) {
        return global_context()->isGlobal(sym);
    }

    // procedure 级

    void CodeBuildMixin::entry_function(func_symbol name, func_info info) {
        auto func = &ir_code()->setup_procedure(name, info);
        set_procedure(func);
    }

    Procedure *CodeBuildMixin::current_procedure() {
        return procedure();
    }


    void CodeBuildMixin::exit_function() {
        set_procedure(nullptr);
    }

    // block 级

    BasicBlock *CodeBuildMixin::newBasicBlock() {
        if (procedure()) {
            auto newBlock = control_flow()->add();
            return newBlock;
        } else {
            LOG_WARN("Get basic when not entry any function");
            return nullptr;
        }
    }

    void CodeBuildMixin::entry_block() {
        auto blk = newBasicBlock();
        set_basic_block(blk);
    }

    void CodeBuildMixin::entry_block(BasicBlock *blk) {
        if (blk) {
            set_basic_block(blk);
        } else {
            LOG_WARN("Set Basic Block as nullptr");
        }
    }

    void CodeBuildMixin::entry_next() {
        auto &curBlk = top_block();
        if (curBlk) {
            if (auto nxt_blk = curBlk->nextBlock()) {
                entry_block(nxt_blk);
            } else {
                curBlk->setNext(newBasicBlock());
                entry_block(curBlk->nextBlock());
            }
        } else {
            curBlk = newBasicBlock();
        }
    }

    void CodeBuildMixin::entry_else() {
        auto &curBlk = top_block();
        if (curBlk) {
            if (auto nxt_blk = curBlk->elseBlock()) {
                entry_block(nxt_blk);
            } else {
                curBlk->setElse(newBasicBlock());
                entry_block(curBlk->elseBlock());
            }
        } else {
            curBlk = newBasicBlock();
        }
    }

    void CodeBuildMixin::push_block() {
        save_basic_block();
    }

    void CodeBuildMixin::save_and_entry(BasicBlock *blk) {
        save_and_set(blk);
    }

    void CodeBuildMixin::save_and_next() {
        push_block();
        entry_next();
    }

    void CodeBuildMixin::save_and_else() {
        push_block();
        entry_else();
    }

    void CodeBuildMixin::pop_block() {
        pop_basic_block();
    }

    BasicBlock *CodeBuildMixin::current_block() const {
        return current_basic_block();
    }

    void CodeBuildMixin::setNext(BasicBlock *blk) {
        if (top_block()) {
            top_block()->setNext(blk);
        } else {
            LOG_WARN("Set next basic block for nullptr ");
        }
    }

    void CodeBuildMixin::setElse(BasicBlock *blk) {
        if (top_block()) {
            top_block()->setElse(blk);
        } else {
            LOG_WARN("Set next else basic block for nullptr");
        }
    }

    BasicBlock *&CodeBuildMixin::top_block() {
        if (_block_stack.empty()) {
            _block_stack.push(nullptr);
        }
        return _block_stack.top();
    }

    var_symbol CodeBuildMixin::nxt_sym() {
        return procedure_context()->nextSymbol();
    }

    var_symbol CodeBuildMixin::nxt_sym(std::string sym) {
        return procedure_context()->nextSymbol(sym);
    }

    var_symbol CodeBuildMixin::nxt_sym(var_symbol sym) {
        return procedure_context()->nextSymbol(sym);
    }

    ProcedureDU &CodeBuildMixin::current_du() {
        return procedure()->du_info();
    }

    bool CodeBuildMixin::is_block_param(var_symbol arg) {
        return current_block()->is_arg(arg);
    }

    void CodeBuildMixin::remove_block_param(var_symbol arg) {
        current_block()->remove_arg(arg);
        current_du().remove_define(arg);
    }

    void CodeBuildMixin::add_block_param(var_symbol arg) {
        current_block()->add_arg(arg);
        current_du().add_block_param(current_block(), arg);
    }

    std::size_t CodeBuildMixin::block_param_index(var_symbol arg) {
        return current_block()->arg_index(arg);
    }

    void CodeBuildMixin::remove_block_param(std::size_t index) {
        auto &arg = current_block()->getArgs()[index];
        current_block()->remove_arg(index);
        current_du().remove_define(arg);
    }

    var_symbol CodeBuildMixin::block_arg(std::size_t index) {
        return current_block()->getArgs()[index];
    }

}