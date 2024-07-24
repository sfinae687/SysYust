//
// Created by LL06p on 24-7-24.
//

#include "IR/ProcedureContext.h"

namespace SysYust::IR {
    ProcedureContext::ProcedureContext(CodeContext &context)
        : global(context)
    {

    }

    bool ProcedureContext::is_param(var_symbol v) const {
        return param_var_set.contains(v);
    }

    std::size_t ProcedureContext::lastRevision(const var_symbol& v) const {
        return lastRevision(v.symbol);
    }

    std::size_t ProcedureContext::lastRevision(const std::string& sym) const {
        return _nxt_revision.at(sym)-1;
    }

    std::size_t ProcedureContext::nextRevision(const var_symbol& v) {
        return nextRevision(v.symbol);
    }

    std::size_t ProcedureContext::nextRevision(const std::string& sym) {
        return _nxt_revision[sym]++;
    }

    var_symbol ProcedureContext::nextSymbol(const var_symbol& v) {
        return nextSymbol(v.symbol);
    }

    var_symbol ProcedureContext::nextSymbol(const std::string& sym) {
        auto nr = _nxt_revision[sym]++;
        return {sym, nr};
    }

    var_symbol ProcedureContext::nextSymbol() {
        return nextSymbol(defaultSymbol);
    }
} // IR