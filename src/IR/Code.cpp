//
// Created by LL06p on 24-7-20.
//

#include "IR/Code.h"

namespace SysYust::IR {
    Procedure &Code::setup_procedure(const func_symbol& name, func_info info) {
        _all_procedure.emplace_back(context, name, info);
        return _all_procedure.back();
    }

    const std::list<Procedure> &Code::procedures() const {
        return _all_procedure;
    }
} // IR