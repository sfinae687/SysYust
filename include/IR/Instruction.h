//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_INSTRUCTION_H
#define SYSYUST_INSTRUCTION_H

#include <variant>
#include <optional>

#include "InstructionUtil.h"

namespace SysYust::IR {

    /**
     * @brief 指令的详细类型标识符
     */
    enum  instruct_type {
        // 占用0位
        INT = 0,

        // 整数
        neg, add, sub, mul, mod, div, rem, i2f, i2b,
        eq, ne, lt, le, gt, ge,
        indexof,

        // 浮点操作
        fneg, fadd, fsub, fmul, fdiv, frem, f2i, f2b,
        feq, fne, flt, fle, fgt, fge,

        // 调用
        call,

        // 控制流
        br, jp, rt,

        // 内存
        alc, ld, st
    };

    /**
     * @brief 指令的归类描述，同一归类的指令由同一个类实现
     */
    enum class instruct_cate {
        out_of_instruct,
        with_2,
        with_1,
        call,
        branch,
        jump,
        ret,
        load,
        store,
        alloc,
        index,
    };

    /**
     * @brief 归类器的实现函数
     */
    constexpr instruct_cate cate(instruct_type t) {
        switch (t) {
            case instruct_type::INT:
                return instruct_cate::out_of_instruct;

            // 一元操作的实现
            case instruct_type::neg:
            case instruct_type::fneg:
            case instruct_type::i2b:
            case instruct_type::i2f:
            case instruct_type::f2i:
            case instruct_type::f2b:
                return instruct_cate::with_1;

            // 跳转
            case instruct_type::jp:
                return instruct_cate::jump;
            case instruct_type::call:
                return instruct_cate::call;
            case instruct_type::br:
                return instruct_cate::branch;
            case instruct_type::rt:
                return instruct_cate::ret;

            // 内存操作
            case instruct_type::ld:
                return instruct_cate::load;
            case instruct_type::st:
                return instruct_cate::store;
            case instruct_type::alc:
                return instruct_cate::alloc;

            // 依赖与类型系统的操作
            // p.s. alc 也依赖于类型系统
            case instruct_type::indexof:
                return instruct_cate::index;
            default:
                return instruct_cate::with_2;
        }
    }

    /**
     * @brief 指令共同基类，提供了类型支持
     */
     template<typename D>
    struct instruct_base {
        explicit instruct_base(instruct_type type)
            : type(type)
        {

        }


        instruct_base(const instruct_base &) = default;
        const instruct_type type;
    };

    /**
     * @brief 带有两个操作数的指令
     */
    struct compute_with_2 : instruct_base<compute_with_2> {

        compute_with_2 (instruct_type type, var_symbol v, operant op1, operant op2);

        const var_symbol assigned;
        const operant opr1;
        const operant opr2;
    };

    /**
     * @brief 带有一个操作数的指令
     */
    struct compute_with_1 : instruct_base<compute_with_2> {

        compute_with_1(instruct_type t, var_symbol v, operant op1, operant op2);
        compute_with_1(instruct_type t, var_symbol v, operant op1);

        const var_symbol assigned;
        const operant opr;
        const operant &opr1{opr}; ///< 兼容接口，辅助泛型编程, 虽然不太可能用得到
        static const operant opr2_base;
        const operant &opr2{opr2_base};
    };


    struct indexOf : instruct_base<indexOf> {

        indexOf(var_symbol assigned, const Type *type, operant index);

        const var_symbol assigned;
        const Type *type;
        const operant ind;
    };

    struct load : instruct_base<load> {

        load(var_symbol in, var_symbol out);

        const var_symbol source;
        const var_symbol target;
    };

    struct store : instruct_base<store> {

        store(var_symbol t, var_symbol s);

        const var_symbol source;
        const var_symbol target;
    };

    struct alloc : instruct_base<alloc> {

        alloc(var_symbol assigned, const Type *type);

        const var_symbol assigned;
        const Type *type;
    };

    struct call_instruct : instruct_base<call_instruct> {

        call_instruct(func_symbol f, std::initializer_list<operant> oprs);

        const func_symbol func;
        const std::vector<operant> args;
    };

    // 基本块结尾命令

    struct branch : instruct_base<branch> {
        explicit branch(operant cond);
        const operant cond;

    };

    struct jump : instruct_base<jump> {
        jump();
    };

    struct ret : instruct_base<ret> {
        ret();
    };

    template<instruct_type t, typename... Args>
    auto inst(Args&&... args) {
        if constexpr (constexpr auto ct = cate(t); ct == instruct_cate::out_of_instruct) {
            assert("The instruct can't be created");
        } else if constexpr (ct == instruct_cate::call) {
            return call_instruct(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::with_1) {
            return compute_with_1(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::with_2) {
            return compute_with_2(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::jump) {
            return jump(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::branch) {
            return branch(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::ret) {
            return ret(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::load) {
            return load(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::store) {
            return store(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::alloc) {
            return alloc(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::index /*always true*/ ) {
            return indexOf(std::forward<Args>(args)...);
        }
    }


} // IR

#endif //SYSYUST_INSTRUCTION_H
