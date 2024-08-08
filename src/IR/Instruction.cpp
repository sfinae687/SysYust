//
// Created by LL06p on 24-7-20.
//

#include <utility>
#include <cassert>

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>

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
        , true_args(true_a)
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
            curT = curT->subtype();
            assert(!curT->isPtr());
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

    operant arg_at(const instruction &it, std::size_t index) {
        switch (static_cast<instruct_cate>(it.index())) {
            case instruct_cate::with_2: {
                auto &inst_2 = std::get<compute_with_2>(it);
                if (index) {
                    return inst_2.opr2;
                } else {
                    return inst_2.opr1;
                }
            }
            case instruct_cate::with_1: {
                auto &inst_1 = std::get<compute_with_1>(it);
                return inst_1.opr;
            }
            case instruct_cate::call: {
                auto &call_inst = std::get<call_instruct>(it);
                return call_inst.args[index];
            }
            case instruct_cate::branch: {
                auto &br_inst = std::get<branch>(it);
                if (index == 0) {
                    return br_inst.cond;
                }
                auto true_size = br_inst.true_args.size();
                if (index - 1 < true_size) {
                    return br_inst.true_args[index - 1];
                } else {
                    return br_inst.false_args[index - 1 - true_size];
                }
            }
            case instruct_cate::jump: {
                auto &jp_inst = std::get<jump>(it);
                return jp_inst.args[index];
            }
            case instruct_cate::ret: {
                auto &ret_inst = std::get<ret>(it);
                return *ret_inst.args;
            }
            case instruct_cate::load: {
                auto &load_inst = std::get<load>(it);
                return load_inst.source;
            }
            case instruct_cate::store: {
                auto &store_inst = std::get<store>(it);
                switch (index) {
                    case 0:
                        return store_inst.target;
                    case 1:
                        return store_inst.source;
                }
            }
            case instruct_cate::index: {
                auto &index_inst = std::get<indexOf>(it);
                if (index == 0) {
                    return index_inst.arr;
                } else {
                    return index_inst.ind[index - 1];
                }
            }
            case instruct_cate::rv_inst: {
                auto &rvi = std::get<RiscVInstruction>(it);
                assert(!rvi.register_arg);
                return std::get<operant>(rvi._args[index]);
            }
            default:
                __builtin_unreachable();
        }
    }

    void set_arg_at(instruction &it, std::size_t index, operant nArg) {
        switch (static_cast<instruct_cate>(it.index())) {
            case instruct_cate::with_2: {
                auto &inst_2 = std::get<compute_with_2>(it);
                if (index == 1) {
                    inst_2.opr2 = nArg;
                } else {
                    inst_2.opr1 = nArg;
                }
                break;
            }
            case instruct_cate::with_1: {
                auto &inst_1 = std::get<compute_with_1>(it);
                inst_1.opr = nArg;
                break;
            }
            case instruct_cate::call: {
                auto &call_inst = std::get<call_instruct>(it);
                call_inst.args[index] = nArg;
                break;
            }
            case instruct_cate::branch: {
                auto &br_inst = std::get<branch>(it);
                if (index == 0) {
                    br_inst.cond = nArg;
                } else if (auto true_size = br_inst.true_args.size(); index - 1 < true_size) {
                    br_inst.true_args[index-1] = nArg;
                } else {
                    br_inst.false_args[index-1-true_size] = nArg;
                }
                break;
            }
            case instruct_cate::jump: {
                auto &jump_inst = std::get<jump>(it);
                jump_inst.args[index] = nArg;
                break;
            }
            case instruct_cate::ret: {
                auto &ret_inst = std::get<ret>(it);
                *ret_inst.args = nArg;
                break;
            }
            case instruct_cate::load: {
                auto &load_inst = std::get<load>(it);
                load_inst.target = nArg.var();
                break;
            }
            case instruct_cate::store: {
                auto &store_inst = std::get<store>(it);
                switch (index) {
                    case 0:
                        store_inst.target = nArg.var();
                        break;
                    case 1:
                        store_inst.source = nArg;
                        break;
                }
                break;
            }
            case instruct_cate::index: {
                auto &ind_inst = std::get<indexOf>(it);
                if (index == 0) {
                    ind_inst.arr = nArg.var();
                } else {
                    ind_inst.ind[index - 1] = nArg;
                }
                break;
            }
            case instruct_cate::rv_inst: {
                auto &rvi = std::get<RiscVInstruction>(it);
                rvi._args[index] = nArg;
                break;
            }
            default:
                __builtin_unreachable();
        }
    }

    std::size_t arg_size(const instruction &it) {
        switch (static_cast<instruct_cate>(it.index())) {
            case instruct_cate::with_2:
            case instruct_cate::load:
                return 2;
            case instruct_cate::with_1:
            case instruct_cate::store:
                return 1;
            case instruct_cate::alloc:
                return 0;
            case instruct_cate::ret:
                if (auto &rt_inst = std::get<ret>(it); rt_inst.args) {
                    return 1;
                } else {
                    return 0;
                }
            case instruct_cate::call: {
                auto &call_inst = std::get<call_instruct>(it);
                return call_inst.args.size();
            }
            case instruct_cate::branch: {
                auto &br_inst = std::get<branch>(it);
                auto true_size = br_inst.true_args.size();
                auto false_size = br_inst.false_args.size();
                return 1 + true_size + false_size;
            }
            case instruct_cate::jump: {
                auto &jp_inst = std::get<jump>(it);
                return jp_inst.args.size();
            }
            case instruct_cate::index: {
                auto &id_inst = std::get<indexOf>(it);
                return 1 + id_inst.ind.size();
            }
            case instruct_cate::rv_inst: {
                auto &rvi = std::get<RiscVInstruction>(it);
                std::size_t arg_count = 0;
                if (rvi.register_arg && rvi._returned.index() != 0) {
                    ++arg_count;
                }
                for (auto &i : rvi._args) {
                    if (i.index() != 0) {
                        ++arg_count;
                    }
                }
                return arg_count;
            }
            default:
                __builtin_unreachable();
        }
    }

    var_symbol RiscVInstruction::assigned() const {
        if (_returned.index() != 1) {
            return {"_", 0};
        } else {
            return std::get<var_symbol>(_returned);
        }
    }

    RiscVInstruction
    RiscVInstruction::assign_register(std::optional<RV_Returned_Value> rt, const std::vector<RV_Argument_Value> &args) {
        assert(!register_arg);
        if (rt.has_value()) {
            return {id, *rt, args};
        }
        else {
            return {id, {}, args};
        }
    }

    RiscVInstruction::RiscVInstruction(RiscVInst id, RV_Returned_Value rt, const std::vector<RV_Argument_Value> &arg)
        : instruct<RiscVInstruction>(rv)
        , register_arg(true)
        , id(id)
        , _returned(rt)
    {
        assert(arg.size() <= 2);
        ranges::copy(arg, _args.begin());
    }

    RiscVInstruction::RiscVInstruction(RiscVInst id, const std::vector<operant> &arg)
        : instruct<RiscVInstruction>(rv)
        , register_arg(false)
        , id(id)
        , _returned()
    {
        assert(arg.size() <= 2);
        ranges::copy(arg, _args.begin());
    }

    RiscVInstruction::RiscVInstruction(RiscVInst id, var_symbol assigned, const std::vector<operant> &arg)
        : instruct<RiscVInstruction>(rv)
        , register_arg(false)
        , id(id)
        , _returned(assigned)
    {
        assert(arg.size() <= 2);
        ranges::copy(arg, _args.begin());
    }
} // IR