//
// Created by LL06p on 24-7-24.
//

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

#include "IR/CodeUtil.h"

namespace SysYust::IR {

    namespace ranges = ::ranges;
    namespace views = ranges::views;

    // IR 级

    void CodeBuildMixin::reset() {
        delete ir_code;
        _code_context = nullptr;
        _current_procedure = nullptr;
        _procedure_context = nullptr;
        while (!_block_stack.empty()) {
            _block_stack.pop();
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

    Code *CodeBuildMixin::getCode() {
        auto rt = ir_code;
        ir_code = nullptr;
        reset();
        return rt;
    }

    bool CodeBuildMixin::is_global(const var_symbol &sym) {
        return _code_context->is_global(sym);
    }

    // procedure 级

    void CodeBuildMixin::entry_function(func_symbol name, func_info info) {
        auto func = &ir_code->setup_procedure(name, info);
        _current_procedure = func;
        _procedure_context = _current_procedure->context();
    }

    Procedure *CodeBuildMixin::current_procedure() {
        return _current_procedure;
    }

    Procedure* CodeBuildMixin::procedure(func_symbol name) {
        auto proc = ranges::find_if(ir_code->procedures(), [name](const auto &i){
            return i.name() == name;
        });
        return std::addressof(*proc);
    }

    void CodeBuildMixin::exit_function() {
        _current_procedure = nullptr;
        _procedure_context = nullptr;
    }

    // block 级

    BasicBlock *CodeBuildMixin::newBasicBlock() {
        if (_current_procedure) {
            auto newBlock = _current_procedure->getGraph().add();
            return newBlock;
        } else {
            LOG_WARN("Get basic when not entry any function");
            return nullptr;
        }
    }

    void CodeBuildMixin::entry_block() {
        auto blk = newBasicBlock();
        entry_block(blk);
    }

    void CodeBuildMixin::entry_block(BasicBlock *blk) {
        if (blk) {
            top_block() = blk;
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
        _block_stack.push(top_block());
    }

    void CodeBuildMixin::save_and_entry(BasicBlock *blk) {
        push_block();
        top_block() = blk;
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
        if (!_block_stack.empty()) {
            _block_stack.pop();
        }
    }

    BasicBlock *CodeBuildMixin::current_block() const {
        if (_block_stack.empty()) {
            return nullptr;
        } else {
            return _block_stack.top();
        }
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

}