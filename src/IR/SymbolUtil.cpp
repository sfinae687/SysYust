//
// Created by LL06p on 24-7-20.
//

#include <fmt/core.h>
#include <cassert>

#include "IR/SymbolUtil.h"

SysYust::IR::compose_type
SysYust::IR::operant_compose(const SysYust::IR::operant &lhs, const SysYust::IR::operant &rhs) {
    if (lhs.symbolType == rhs.symbolType) {
        if (lhs.symbolType == symbol_type::v) {
            return compose_type::rr;
        } else {
            return compose_type::ii;
        }
    } else {
        return compose_type::ri;
    }
}

SysYust::IR::var_symbol::label_type SysYust::IR::var_symbol::full() const {
    return fmt::format("{}{}{}", symbol, seq, revision);
}

SysYust::IR::im_symbol::label_type SysYust::IR::im_symbol::full() const {
    return *this | [](auto i) {return std::to_string(i);};
}

SysYust::IR::im_symbol::im_symbol(SysYust::IR::im_symbol::flag f, const SysYust::IR::Type *type)
    : data(f)
    , type(type)
{

}

SysYust::IR::operant::operant(SysYust::IR::var_symbol val)
    : _symbolType(symbol_type::v)
    , _type(val.type)
    , _symbol(val)
{

}

SysYust::IR::operant::operant(SysYust::IR::im_symbol im_val)
    : _symbolType(symbol_type::im)
    , _type(im_val.type)
    , _symbol(im_val)
{
    if constexpr (strict_check_flag) {
        assert(type->isBasic());
    }
}

SysYust::IR::operant::label_type SysYust::IR::operant::full() const {
    return *this | [](auto i) {return i.full();};
}

SysYust::IR::var_symbol SysYust::IR::operant::var() const {
    return std::get<var_symbol>(symbol);
}

SysYust::IR::im_symbol SysYust::IR::operant::im() const {
    return std::get<im_symbol>(symbol);
}

SysYust::IR::func_symbol::label_type SysYust::IR::func_symbol::full() const {
    return symbol;
}
