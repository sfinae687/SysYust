//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_INSTRUCTION_H
#define SYSYUST_INSTRUCTION_H

#include <variant>
#include <optional>
#include <cassert>

#include "IR/RISCV_inst.h"
#include "IR/SymbolUtil.h"

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
    struct RiscVInstruction;

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
     *  - alc 操作返回该类型的指针
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

        // 指针操作
        indexof,

        // 浮点操作
        fneg, fadd, fsub, fmul, fdiv, frem, f2i, f2b,
        feq, fne, flt, fle, fgt, fge,

        // 调用
        call,

        // 控制流
        br, jp, rt,

        // 内存
        alc, ld, st,

        // 其他
        rv,
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
        rv_inst,
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
    template<>
    inline constexpr instruct_cate inst_cate<RiscVInstruction> = instruct_cate::rv_inst;

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

            case instruct_type::rv:
                return instruct_cate::rv_inst;

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

    template<typename T>
    concept have_assigned = requires (std::remove_cvref_t<T> t) {
        t.assigned;
        requires not std::is_same_v<T, RiscVInstruction>; // 为啥有 concept 没有 <concepts>
    };

    struct get_assigned_fn {
        template<typename T>
        std::optional<var_symbol> operator() (const T &i) const {
            if constexpr (have_assigned<T>) {
                return i.assigned;
            } else if constexpr (std::is_same_v<T, RiscVInstruction>) {
                return i.assigned();
            }
            return std::nullopt;
        };
    };
    inline constexpr get_assigned_fn get_assigned{};

    struct set_assigned_fn {
        template<typename T>
        void operator() (T &i, var_symbol nVar) const {
            if constexpr (have_assigned<T>) {
                i.assigned = nVar;
            } else if constexpr (std::is_same_v<T, RiscVInstruction>) {
                i._returned = nVar;
            }
        }
    };
    inline constexpr set_assigned_fn set_assigned;

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

        instruct_type type;
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

        constexpr friend bool operator== (const instruct &lhs, const instruct &rhs) {
            return lhs.cateId == rhs.cateId;
        }
    };

    /**
     * @brief 带有两个操作数的指令
     * @details
     * # 类型传播行为
     *
     * 除了比较操作外按照参数类型传播，比较操作返回类型为整数。
     */
    struct compute_with_2 : instruct<compute_with_2> {

        compute_with_2 (instruct_type type, var_symbol v, operant op1, operant op2);

        var_symbol assigned;
        operant opr1;
        operant opr2;
    };

    /**
     * @brief 带有一个操作数的指令
     * @details
     * # 类型传播行为
     *
     * 除了 i2f，fneg返回浮点类型，其他返回整数类型
     */
    struct compute_with_1 : instruct<compute_with_2> {

        compute_with_1(instruct_type t, var_symbol v, operant op1);

        var_symbol assigned;
        operant opr;
    };

    static_assert(std::is_copy_assignable_v<compute_with_1>);


    /**
     * @brief 获取数组成员的指针
     * @todo 重新命名为更相关的名字
     * @details
     * # 类型传播行为
     *
     * arr_like 类型的第 index 长度 深度的子类型的指针类型
     */
    struct indexOf : instruct<indexOf> {

        indexOf(var_symbol assigned, var_symbol arr_like, std::vector<operant> index);

        indexOf& operator= (const indexOf &oth) = default;

        friend bool operator== (const indexOf &lhs, const indexOf &rhs) = default;

        var_symbol assigned;
        var_symbol arr;
        std::vector<operant> ind;
    };

    static_assert(std::is_copy_assignable_v<indexOf>);

    /**
     * @brief 读取操作
     * @details
     * # 类型传递操作
     *
     * 操作数类型的子类型，操作数应为指针类型。
     */
    struct load : instruct<load> {

        load(var_symbol t, var_symbol s);

        load(const load &oth)
            : instruct<load>(instruct_type::ld)
            , source(oth.source)
            , target(oth.target)
        {

        }

        load& operator= (const load &oth) {
            source = oth.source;
            target = oth.target;
            return *this;
        }

        var_symbol source; ///< 读取的位置，必须为指针类型
        var_symbol target; ///< 读取到的变量名称
        var_symbol &assigned{target};
    };

    static_assert(std::is_copy_assignable_v<load>);

    /**
     * @brief 写入操作
     * @details
     *
     * 参数索引：
     *  - 0，target，
     *  - 1，source
     *
     * # 类型传播操作
     *
     * 不自动生成符号，也不控制传播，但是检查类型匹配。
     */
    struct store : instruct<store> {

        store(var_symbol t, operant s);

        store(const store &oth)
            : store(oth.target, oth.source)
        {

        }

        store& operator= (const store &oth) {
            source = oth.source;
            target = oth.target;
            return *this;
        }

        operant source;
        var_symbol target; ///< 写入的位置，必须为指针类型
//        var_symbol &assigned{target};
    };

    static_assert(std::is_copy_assignable_v<store>);

    /**
     * @brief 获取一个特定类型的指针
     * @details
     * # 类型传播
     *
     * 给定类型的指针类型
     */
    struct alloc : instruct<alloc> {

        alloc(var_symbol v, const Type *type);

        var_symbol assigned;
        const Type *type;
    };

    /**
     * @brief 函数调用操作
     * @details
     * # 类型传播
     *
     * 无法根据符号获取类型，需要获取上下文
     */
    struct call_instruct : instruct<call_instruct> {

        call_instruct(var_symbol v, func_symbol f, std::vector<operant> opr);

        var_symbol assigned; ///< 需要根据上下文确定符号 v 的类型
        func_symbol func;
        arg_list args;
    };

    static_assert(std::is_copy_assignable_v<call_instruct>);

    // 基本块结尾命令

    struct branch : instruct<branch> {
        explicit branch(operant cond, arg_list true_a, arg_list false_a);
        operant cond;
        arg_list true_args;
        arg_list false_args;
    };

    static_assert(std::is_copy_assignable_v<branch>);

    struct jump : instruct<jump> {
        explicit jump(arg_list aArg);
        arg_list args;
    };

    static_assert(std::is_copy_assignable_v<jump>);

    struct ret : instruct<ret> {
        explicit ret(std::optional<operant> aArg = std::nullopt);
        std::optional<operant> args;
    };

    static_assert(std::is_copy_assignable_v<ret>);

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
        } else if constexpr (ct == instruct_cate::rv_inst) {
            return RiscVInstruction(std::forward<Args>(args)...);
        } else {
            __builtin_unreachable();
        }
    }

    /**
     * @brief RiscV 指令的存储类型，目前支持一个返回值和追多两个参数值
     * @details 具有 var_symbol 参数模式和寄存器参数模式，两种模式下 arg_size 均工作正常，不过在寄存器参数模式下返回值会被视为参数。
     * 仅在 var_symbol 参数模式下arg_at和set_arg_at工作正常。
     */
    struct RiscVInstruction : public instruct<RiscVInstruction> {

        /**
         * @brief 变量模式的构造函数,带有返回符号
         */
        RiscVInstruction(RiscVInst id, var_symbol assigned, const std::vector<operant> &arg);
        /**
         * @brief 符号模式的构造函数,带有返回符号
         */
        RiscVInstruction(RiscVInst id, const std::vector<operant> &arg);
        /**
         * @brief 寄存器模式的构造函数
         * @param rt 返回寄存器,如果没有传默认构造 `{}`
         */
        RiscVInstruction(RiscVInst id, RV_Returned_Value rt, const std::vector<RV_Argument_Value> &arg);

        RiscVInstruction(const RiscVInstruction &) = default;
        RiscVInstruction(RiscVInstruction &&) = default;

        RiscVInstruction& operator= (const RiscVInstruction &) = default;
        RiscVInstruction& operator= (RiscVInstruction &&) = default;

        /**
         * @brief 获取一个分配了寄存器的版本的RiscVInstruction
         * @detials 这个函数的预期用法是获取一个新的对象,并用它通过带有上下文(比如定义-用例追踪)的方式重设原有的对象.
         */
        RiscVInstruction assign_register(std::optional<RV_Returned_Value> rt, const std::vector<RV_Argument_Value> &args);

        bool register_arg = false;

        RiscVInst id;
        std::variant<std::monostate, var_symbol, RV_Returned_Value> _returned{};
        std::array<std::variant<std::monostate, operant, RV_Argument_Value>, 2> _args{};

        /**
         * @brief 获取符号模式的返回符号
         * @note 符号模式的参数通过 arg_size, arg_at, set_arg_at 处理
         */
        [[nodiscard]] var_symbol assigned() const;

        /**
         * @brief 获取寄存器返回值的方法
         */
        template<RiscVInst inst>
        auto& returned() {
            assert(register_arg);
            using return_type = typename RV_Type<inst>::return_type;
            return std::get<return_type>(_returned);
        };

        /**
         * @brief 获取寄存器参数的方法
         */
        template<RiscVInst inst, std::size_t N>
        auto& arg() {
            assert(register_arg);
            using arg_type = std::tuple_element_t<N, typename RV_Type<inst>::args>();
            return std::get<arg_type>(std::get<RV_Argument_Value>(_args[N]));
        }

    };


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
            indexOf,
            RiscVInstruction
            >;
    static_assert(std::is_copy_constructible_v<instruction>);
    static_assert(std::is_copy_assignable_v<instruction>);

    [[maybe_unused]] std::size_t arg_size(const instruction &it);

    operant arg_at(const instruction &it, std::size_t index);

    void set_arg_at(instruction &it, std::size_t index, operant nArg);

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
