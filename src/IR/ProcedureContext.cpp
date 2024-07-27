//
// Created by LL06p on 24-7-24.
//

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

#include "IR/ProcedureContext.h"

namespace SysYust::IR {

    namespace ranges = ::ranges;
    namespace views = ranges::views;

    ProcedureContext::ProcedureContext(CodeContext &context)
        : global(context)
    {

    }

    bool ProcedureContext::is_param(var_symbol v) const {
        return ranges::find(param_var_set, v) != param_var_set.end();
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

    std::size_t ProcedureContext::param_index(var_symbol v) const {
        auto founded = ranges::find(param_var_set, v);
        if (founded != param_var_set.end()) {
            return founded - param_var_set.begin();
        } else {
            return -1;
        }
    }
} // IR