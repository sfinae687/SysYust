//
// Created by LL06p on 24-7-20.
//

#include "IR/Code.h"
#include "Logger.h"

namespace SysYust::IR {
    Procedure &Code::setup_procedure(const func_symbol& name, func_info info) {
        _all_procedure.emplace_back(context, name, info);
        return _all_procedure.back();
    }

    std::list<Procedure> & Code::procedures() {
        return _all_procedure;
    }

    const std::list<Procedure> &Code::procedures() const {
        return _all_procedure;
    }

    void Code::set_var(var_symbol var, InitInfo init) {
        if (is_global_var(var)) {
            auto type = context.globalType(var);
            if (type == init.value().type) {
                global_var_value[var] = init;
            } else {
                LOG_WARN("The initial value isn't match the variable type for variable {}", var.full());
            }
        } else {
            if (!init.is_list()) {
                var.type = init.value().type;
            }
            context.addGlobal(var);
            global_var_value[var] = init;
        }
    }

    void Code::unset_var(var_symbol var) {
        context.removeGlobal(var);
        global_var_value.erase(var);
    }

    bool Code::is_global_var(var_symbol var) const {
        return context.isGlobal(var);
    }
} // IR