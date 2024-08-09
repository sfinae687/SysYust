#ifndef SYSYUST_InstSel_H
#define SYSYUST_InstSel_H

#include <iostream>
#include <type_traits>
#include <vector>

#include "IR/BasicBlock.h"
#include "IR/Code.h"
#include "IR/CodeUtil.h"
#include "IR/Instruction.h"
#include "IR/Procedure.h"
#include "IR/RISCV_inst.h"
#include "IR/SymbolUtil.h"
#include "fmt/format.h"
#include "IRPrinter/IRPrinter.h"
namespace SysYust::IR {

class InstSel {
   public:
    instruction sel_with_1(compute_with_1 &inst);

    instruction emitADDI(var_symbol op1, int op2);

    var_symbol ldfimm(operant op);

    instruction sel_with_2(compute_with_2 &inst);

    instruction sel_load(load &inst);

    instruction sel_store(store &inst);

    instruction sel_index(indexOf &inst);

    Code *run(Code *code);

    BasicBlock *_curBB;
    Procedure *_cur_proc;
    BasicBlock::iterator _insert_point;

    void set_insert_point(BasicBlock::iterator it) {
        _insert_point = it;
    }

    void insert(instruction inst) {
        _insert_point = _curBB->insert(_insert_point, inst);
        _cur_proc->track_inst(_curBB, _insert_point);
    }

    template <typename T>
    operant try_cast(T &&t) {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> ||
                      std::is_same_v<unsigned long, T>) {
            return im_symbol(t);
        } else {
            return t;
        }
    }

    template <RiscVInst T, typename... Args>
    var_symbol createInst(Args... args) {
        auto ops = std::vector<operant>{try_cast(std::forward<Args>(args))...};
        auto inst =
            IR::inst<rv>(T, _cur_proc->context()->nextSymbol(), std::move(ops));
        insert(inst);
        std::cout << "createInst " << fmt::to_string(instruction{inst}) << std::endl;
        return inst.assigned();
    }

    template <RiscVInst T, typename... Args>
    instruction Inst(Args... args) {
        auto ops = std::vector<operant>{try_cast(std::forward<Args>(args))...};
        return IR::inst<rv>(T, _cur_proc->context()->nextSymbol(), std::move(ops));
    }
};

}  // namespace SysYust::IR

#endif