//
// Created by LL06p on 24-7-24.
//

#include <stdexcept>

#include <fmt/core.h>

#include "utility/Logger.h"
#include "IR/CodeContext.h"

namespace SysYust::IR {
    const Type *CodeContext::get_fun_type(const func_symbol& sym) const {
        if (func_set.contains(sym)) {
            return func_set.at(sym).type;
        } else {
            LOG_ERROR("IR:: lack function info for {}", sym.full());
            throw std::out_of_range(fmt::format("Can't find the symbol of {}", sym.full()));
        }
    }

    func_info CodeContext::get_func(const func_symbol& sym) const {
        if (func_set.contains(sym)) {
            return func_set.at(sym);
        } else {
            LOG_ERROR("IR:: lack function info for {}", sym.full());
            throw std::out_of_range(fmt::format("Can't find the symbol of {}", sym.full()));
        }
    }

    void CodeContext::set_func(const func_symbol& sym, func_info func) {
        LOG_INFO("IR:: Set function {}", sym.full());
        func_set[sym] = func;
    }

    bool CodeContext::is_global(const var_symbol& sym) const {
        return global_vars.contains(sym);
    }
} // IR