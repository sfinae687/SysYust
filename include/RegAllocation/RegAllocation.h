#ifndef SYSYUST_REGALLOCATION_H
#define SYSYUST_REGALLOCATION_H

#include "IR/BasicBlock.h"
#include "IR/Code.h"
#include "IR/Def-Use/ProcedureDU.h"
#include "IR/Procedure.h"
#include "IR/SymbolUtil.h"
namespace SysYust::IR {

using DUInst_list_type = std::map<instruction*, DUInst>;

class RegAllocation {
    auto getInstArgs(instruction &inst) {
        return (*_param_list)[&inst].args;
    }

    void colorRec(BasicBlock *bb) {
        auto assigned = 1;
        for (auto inst : *bb) {
            for (auto entry : getInstArgs(inst)) {
                auto arg = entry.opr();
                if (lastUse(arg)) {
                    assigned.earse(col[arg]);
                }
            }
            auto ret = std::visit([](auto &&inst) {
                if constexpr (have_assigned<decltype(inst)>) return inst.assigned;
                else return var_symbol{};
            }, inst);

            if (ret != var_symbol{}) {
                col[ret] = pickColor(assigned);
            }
        }

        for (auto C : idom(B)) {
            colorRec(C);
        }
    }

    void displace() {
        
    }

    void runOnProcedure(Procedure * proc) {
        auto head = proc->getGraph().head();
        _param_list = &proc->du_info().getInstList();
        colorRec(head);
    }

    Code *run(Code *code) {
        for (auto proc : code->procedures()) {
            runOnProcedure(proc);
        }
    }

    DUInst_list_type *_param_list;
};

} // SysYust::IR

#endif // SYSYUST_REGALLOCATION_H