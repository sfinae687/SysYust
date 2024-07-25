//
// Created by LL06p on 24-7-20.
//

#include <utility>
#include <cassert>

#include "IR/Instruction.h"


namespace SysYust::IR {

    compute_with_2::compute_with_2(instruct_type type, var_symbol v, operant op1, operant op2)
        : instruct(type)
        , assigned(std::move(v))
        , opr1(std::move(op1))
        , opr2(std::move(op2))
        {

    }

    compute_with_1::compute_with_1(instruct_type t, var_symbol v, operant op1, operant op2)
        : instruct(t)
        , assigned(std::move(v))
        , opr(std::move(op1))
        {

    }

    compute_with_1::compute_with_1(instruct_type t, var_symbol v, operant op1)
        : instruct(t)
        , assigned(std::move(v))
        , opr(std::move(op1))
        {

    }


    call_instruct::call_instruct(func_symbol f, std::initializer_list<operant> oprs)
        : instruct(instruct_type::call)
        , func(f)
        , args(oprs)
        {

    }

    branch::branch(operant cond, std::initializer_list<operant> true_a, std::initializer_list<operant> false_a)
        : instruct(instruct_type::br)
        , cond(std::move(cond))
        , ture_args(true_a)
        , false_args(false_a)
    {

    }

    jump::jump(std::initializer_list<operant> aArg)
        : instruct(instruct_type::jp)
        , args(std::initializer_list<operant>())
    {

    }

    ret::ret(std::optional<operant> aArg)
        : instruct(instruct_type::rt)
        , args(std::move(aArg))
    {

    }

    indexOf::indexOf(var_symbol assigned, const Type *type, operant index)
        : instruct<indexOf>(indexof)
        , assigned(std::move(assigned))
        , type(type)
        , ind(std::move(index))
    {
        if constexpr (strict_check_flag) {
            assert(type->isArr());
        }
    }

    alloc::alloc(var_symbol assigned, const Type *type)
        : instruct<struct alloc>(alc)
        , assigned(std::move(assigned))
        , type(type)
    {
        if constexpr (strict_check_flag) {
            // nothing to check
        }
    }

    load::load(var_symbol t, var_symbol s)
        : instruct<load>(ld)
        , source(std::move(s))
        , target(std::move(t))
    {
        if constexpr (strict_check_flag) {
            assert(source.type->isPtr());
            assert(target.type->isScalar());
        }
    }

    store::store(var_symbol t, var_symbol s)
        : instruct<store>(st)
        , source(std::move(s))
        , target(std::move(t))
    {
        if constexpr (strict_check_flag) {
            assert(target.type->isPtr());
            assert(source.type->isScalar());
        }
    }
} // IR