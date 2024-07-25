//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_INSTRUCTION_H
#define SYSYUST_INSTRUCTION_H

#include <variant>
#include <optional>
#include <cassert>

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

    using arg_list = std::vector<operant>;

    /**
     * @brief 指令的详细类型标识符
     * @details
     *  # 类型传播规则
     *
     *  - INT 无效指令
     *  - 整数分区的所有操作结果为整数
     *  - 浮点分区中转换操作和比较操作为整数，其他为浮点数
     *  - 调用操作取决于上下文
     *  - indexof 操作返回数组类型子类型的指针
     *  - alc 操作返回该类型的指针 @todo 待决
     *  - ld 生成的符号类型为参数指针的根类型
     *  - st 不自动生成符号，也不自动传播类型
     *
     *  @todo 重命名indexof
     */
    enum  instruct_type {
        // 占用0位
        INT = 0,

        // 整数
        neg, add, sub, mul, mod, divi, rem, i2f, i2b,
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
     * @brief 返回类型识别支持
     */
    constexpr Type::TypeId getTypeId(instruct_type i) {
        switch (i) {
            case INT:
            case br:
            case jp:
                return Type::none;
            case call:
            case rt:
            case ld:
            case st:
                return Type::dyn;
            default:
                break;
        }
        if (i < indexof) {
            if (i == i2f) {
                return Type::f;
            } else {
                return Type::i;
            }
        } else if (i == indexof) {
            return Type::ptr;
        } else if (i <= frem) {
            return Type::f;
        } else if (i <= fge) {
            return Type::i;
        }
        __builtin_unreachable();
    }

    /**
     * @brief 指令的归类描述，同一归类的指令由同一个类实现
     */
    enum class instruct_cate {
        out_of_instruct = -1,
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
     * @brief 指令类型到归类序号的转换
     */
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

        compute_with_1(instruct_type t, var_symbol v, operant op1);

        const var_symbol assigned;
        const operant opr;
        const operant &opr1{opr}; ///< 兼容接口，辅助泛型编程, 虽然不太可能用得到
    };


    struct indexOf : instruct<indexOf> {

        indexOf(var_symbol assigned, var_symbol arr_like, std::vector<operant> index);

        const var_symbol assigned;
        const var_symbol arr;
        const std::vector<operant> ind;
    };

    struct load : instruct<load> {

        load(var_symbol t, var_symbol s);

        const var_symbol source; ///< 读取的位置，必须为指针类型
        const var_symbol target; ///< 读取到的变量名称
        const var_symbol &assigned{target};
    };

    struct store : instruct<store> {

        store(var_symbol t, operant s);

        const operant source;
        const var_symbol target; ///< 写入的位置，必须为指针类型
        const var_symbol &assigned{target};
    };

    struct alloc : instruct<alloc> {

        alloc(var_symbol v, const Type *type);

        const var_symbol assigned;
        const Type *type;
    };

    struct call_instruct : instruct<call_instruct> {

        call_instruct(var_symbol v, func_symbol f, std::vector<operant> opr);

        const var_symbol assigned; ///< 需要根据上下文确定符号 v 的类型
        const func_symbol func;
        const arg_list args;
    };

    // 基本块结尾命令

    struct branch : instruct<branch> {
        explicit branch(operant cond, arg_list true_a, arg_list false_a);
        const operant cond;
        arg_list ture_args;
        arg_list false_args;
    };

    struct jump : instruct<jump> {
        jump(arg_list aArg);
        arg_list args;
    };

    struct ret : instruct<ret> {
        explicit ret(std::optional<operant> aArg = std::nullopt);
        std::optional<operant> args;
    };

    /**
     * @brief 用于创建一条指令的模板函数,不建议直接使用
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
            assert("The instruct can't be created"&&false);
        } else if constexpr (ct == instruct_cate::call) {
            return call_instruct(std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::with_1) {
            return compute_with_1(t, std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::with_2) {
            return compute_with_2(t, std::forward<Args>(args)...);
        } else if constexpr (ct == instruct_cate::jump) {
            return jump(std::initializer_list<operant>());
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
        } else {
            __builtin_unreachable();
        }
    }

    using instruction = std::variant<
            compute_with_2,
            compute_with_1,
            call_instruct,
            branch,
            jump,
            ret,
            load,
            store,
            alloc,
            indexOf
            >;
    enum class gateway_type {
        branch = 1,
        jump,
        ret
    };
    using gateway_inst = std::variant<
            std::monostate,
            branch,
            jump,
            ret
            >;

} // IR

#endif //SYSYUST_INSTRUCTION_H
