//
// Created by LL06p on 24-7-20.
//

#include <fmt/core.h>
#include <cassert>

#include "IR/InstructionUtil.h"

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

SysYust::IR::im_symbol::im_symbol(SysYust::IR::i32 val)
    : type(Type::get(Type::i))
    , data(val)
{

}

SysYust::IR::im_symbol::im_symbol(SysYust::IR::f32 val)
    : type(Type::get(Type::f))
    , data(val)
{

}

SysYust::IR::im_symbol::label_type SysYust::IR::im_symbol::full() const {
    return *this | [](auto i) {return std::to_string(i);};
}

SysYust::IR::operant::operant(SysYust::IR::var_symbol val)
    : symbolType(symbol_type::v)
    , type(get<var_symbol>(symbol).type)
    , symbol(val)
{

}

SysYust::IR::operant::operant(SysYust::IR::im_symbol im_val)
    : symbolType(symbol_type::im)
    , type(get<var_symbol>(symbol).type)
    , symbol(im_val)
{
    if constexpr (strict_check_flag) {
        assert(type->isScalar());
    }
}

SysYust::IR::operant::label_type SysYust::IR::operant::full() const {
    return *this | [](auto i) {return i.full();};
}

SysYust::IR::func_symbol::label_type SysYust::IR::func_symbol::full() const {
    return symbol;
}
