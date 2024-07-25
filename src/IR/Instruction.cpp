//
// Created by LL06p on 24-7-20.
//

#include <utility>
#include <cassert>

#include "IR/Instruction.h"


namespace SysYust::IR {

    compute_with_2::compute_with_2(instruct_type type, var_symbol v, operant op1a, operant op2a)
        : instruct(type)
        , assigned({v.symbol, v.revision, [&] {
            if (f2i <= type && type <= fge) {
                return Type::get(Type::i);
            } else {
                return op1a.type;
            }
        }()})
        , opr1(std::move(op1a))
        , opr2(std::move(op2a))
    {

    }

    compute_with_1::compute_with_1(instruct_type t, var_symbol v, operant op1)
        : instruct(t)
        , assigned({v.symbol, v.revision, [&] {
            if (t == i2f || t == fneg) {
                return Type::get(Type::f);
            } else {
                return Type::get(Type::i);
            }
        }()})
        , opr(std::move(op1))
    {

    }

    call_instruct::call_instruct(var_symbol v, func_symbol f, std::vector<operant> opr)
        : instruct(instruct_type::call)
        , assigned(v)
        , func(std::move(f))
        , args(std::move(opr))
    {

    }

    branch::branch(operant cond, std::vector<operant> true_a, std::vector<operant> false_a)
        : instruct(instruct_type::br)
        , cond(std::move(cond))
        , ture_args(true_a)
        , false_args(false_a)
    {

    }

    jump::jump(std::vector<operant> aArg)
        : instruct(instruct_type::jp)
        , args(std::initializer_list<operant>())
    {

    }

    ret::ret(std::optional<operant> aArg)
        : instruct(instruct_type::rt)
        , args(std::move(aArg))
    {

    }

    indexOf::indexOf(var_symbol assigned, var_symbol arr_like, std::vector<operant> index)
        : instruct<indexOf>(indexof)
        , assigned({assigned.symbol, assigned.revision, [&]{
            // 计算结果类型
            auto curT = arr_like.type;
            for (int i=0; i<index.size(); ++i) {
                curT = curT->subtype();
            }
            return Type::get(Type::ptr, curT);
        }() })
        , arr(arr_like)
        , ind(std::move(index))
    {
        if constexpr (strict_check_flag) {
            assert(arr.type->isArr() || arr.type->isPtr());
        }
    }

    alloc::alloc(var_symbol v, const Type *type)
        : instruct<struct alloc>(alc)
        , assigned({v.symbol, v.revision, Type::get(Type::ptr, type)})
        , type(type)
    {
        if constexpr (strict_check_flag) {
            // nothing to check
        }
    }

    load::load(var_symbol t, var_symbol s)
        : instruct<load>(ld)
        , source(std::move(s))
        , target({t.symbol, t.revision, s.type->subtype()})
    {
        if constexpr (strict_check_flag) {
            assert(source.type->isPtr());
            assert(target.type->isBasic());
        }
    }

    store::store(var_symbol t, operant s)
        : instruct<store>(st)
        , source(std::move(s))
        , target(std::move(t))
    {
        if constexpr (strict_check_flag) {
            assert(target.type->isPtr());
            assert(source.type->isBasic());
        }
    }
} // IR