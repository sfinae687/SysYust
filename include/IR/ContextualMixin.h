//
// Created by LL06p on 24-7-26.
//

#ifndef SYSYUST_CONTEXTUALMIXIN_H
#define SYSYUST_CONTEXTUALMIXIN_H

#include "Logger.h"
#include "IR/TypeUtil.h"
#include "IR/SymbolUtil.h"
#include "IR/ProcedureContext.h"
#include "IR/CodeContext.h"
#include "IR/Code.h"

#include <stack>
#include <cassert>

namespace SysYust::IR {

    class ContextualMixin {
    protected:

        ContextualMixin() = default;
        ContextualMixin(Code *code, Procedure *proc=nullptr);

        [[nodiscard]] Code* ir_code() const;
        [[nodiscard]] CodeContext* global_context() const;
        void set_context(Code *code, Procedure *proc=nullptr);
        void clear_context();

        [[nodiscard]] Procedure* procedure() const;
        [[nodiscard]] Procedure *seekProcedure(func_symbol sym) const;
        ProcedureContext* procedure_context();
        ControlFlow* control_flow() const;
        void set_procedure(Procedure *proc);

        [[nodiscard]] BasicBlock* current_basic_block() const;
        void set_basic_block(BasicBlock *blk);
        void next_basic_block();
        void else_basic_block();
        void save_basic_block();
        void save_and_set(BasicBlock *blk);
        void save_and_next();
        void save_and_else();
        void pop_basic_block();

    protected:
        std::stack<BasicBlock*> _block_stack{};
        CodeContext *_code_context = nullptr;
        Procedure *_current_procedure = nullptr;
        ProcedureContext *_procedure_context = nullptr;
        Code *_ir_code = nullptr;
    };

} // IR

#endif //SYSYUST_CONTEXTUALMIXIN_H
