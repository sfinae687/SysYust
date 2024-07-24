//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_INSTRUCTION_H
#define SYSYUST_INSTRUCTION_H

#include <variant>
#include <optional>

#include "SymbolUtil.h"

namespace SysYust::IR {

    struct undef_inst;
    struct compute_with_2;
    struct compute_with_1;
    struct indexOf;
    struct load;
    struct store;
    struct alloc;
    struct call_instruct;
    struct branch;
    struct jump;
    struct ret;

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

    template<typename Inst>
    inline constexpr instruct_cate inst_cate = instruct_cate::out_of_instruct;
    template<>
    inline constexpr instruct_cate inst_cate<compute_with_2> = instruct_cate::with_2;
    template<>
    inline constexpr instruct_cate inst_cate<compute_with_1> = instruct_cate::with_1;
    template<>
    inline constexpr instruct_cate inst_cate<call_instruct> = instruct_cate::call;
    template<>
    inline constexpr instruct_cate inst_cate<branch> = instruct_cate::branch;
    template<>
    inline constexpr instruct_cate inst_cate<jump> = instruct_cate::jump;
    template<>
    inline constexpr instruct_cate inst_cate<ret> = instruct_cate::ret;
    template<>
    inline constexpr instruct_cate inst_cate<load> = instruct_cate::load;
    template<>
    inline constexpr instruct_cate inst_cate<store> = instruct_cate::store;
    template<>
    inline constexpr instruct_cate inst_cate<alloc> = instruct_cate::alloc;
    template<>
    inline constexpr instruct_cate inst_cate<indexOf> = instruct_cate::index;

    constexpr bool is_gateway(instruct_cate c) {
        if (c == instruct_cate::jump
        || c == instruct_cate::branch
        || c == instruct_cate::ret
        ) {
            return true;
        } else {
            return false;
        }
    }

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

    struct instruct_base {

        explicit instruct_base(instruct_type type)
            : type(type)
        {

        }
        instruct_base(const instruct_base &) = default;

        [[nodiscard]] bool is_gateway() const {
            return type == br
                   || type == jp
                   || type == rt;
        }

        [[nodiscard]] bool is_mem() const {
            return type == ld
                   || type == st
                   || type == alc;
        }

        const instruct_type type;
    };

    /**
     * @brief 指令共同基类，提供了类型支持
     */
     template<typename D>
    struct instruct : instruct_base {

        static constexpr instruct_cate cateId = inst_cate<D>;

        explicit instruct(instruct_type type)
            : instruct_base(type)
        {

        }
        instruct(const instruct&) = default;
    };

    /**
     * @brief 带有两个操作数的指令
     */
    struct compute_with_2 : instruct<compute_with_2> {

        compute_with_2 (instruct_type type, var_symbol v, operant op1, operant op2);

        const var_symbol assigned;
        const operant opr1;
        const operant opr2;
    };

    /**
     * @brief 带有一个操作数的指令
     */
    struct compute_with_1 : instruct<compute_with_2> {

        compute_with_1(instruct_type t, var_symbol v, operant op1, operant op2);
        compute_with_1(instruct_type t, var_symbol v, operant op1);

        const var_symbol assigned;
        const operant opr;
        const operant &opr1{opr}; ///< 兼容接口，辅助泛型编程, 虽然不太可能用得到
        static const operant opr2_base;
        const operant &opr2{opr2_base};
    };


    struct indexOf : instruct<indexOf> {

        indexOf(var_symbol assigned, const Type *type, operant index);

        const var_symbol assigned;
        const Type *type;
        const operant ind;
    };

    struct load : instruct<load> {

        load(var_symbol in, var_symbol out);

        const var_symbol source;
        const var_symbol target;
    };

    struct store : instruct<store> {

        store(var_symbol t, var_symbol s);

        const var_symbol source;
        const var_symbol target;
    };

    struct alloc : instruct<alloc> {

        alloc(var_symbol assigned, const Type *type);

        const var_symbol assigned;
        const Type *type;
    };

    struct call_instruct : instruct<call_instruct> {

        call_instruct(func_symbol f, std::initializer_list<operant> oprs);

        const func_symbol func;
        const std::vector<operant> args;
    };

    // 基本块结尾命令

    struct branch : instruct<branch> {
        explicit branch(operant cond);
        const operant cond;

    };

    struct jump : instruct<jump> {
        jump();
    };

    struct ret : instruct<ret> {
        ret();
    };

    /**
     * @brief 用于创建一条指令的模板函数
     * @details
     *
     *      # usage
     *
     *      ```c++
     *          IR::inst<add>(assigned, op_a, op_b);
     *      ```
     */
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

    using instruction = std::variant<
            compute_with_2,
            compute_with_1,
            indexOf,
            load,
            store,
            alloc,
            call_instruct,
            branch,
            jump,
            ret
            >;
    using gateway_inst = std::variant<
            std::monostate,
            branch,
            jump,
            ret
            >;

} // IR

#endif //SYSYUST_INSTRUCTION_H
