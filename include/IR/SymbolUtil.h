///@file Instruction 的辅助工具，主要为符号的实现

#ifndef SYSYUST_SYMBOLUTIL_H
#define SYSYUST_SYMBOLUTIL_H

#include <utility>
#include <type_traits>
#include <functional>
#include <variant>
#include <string>

#include "IR/TypeUtil.h"

#define IR_DEBUG

#ifdef NDEBUG
#undef IR_DEBUG
#endif

#ifdef IR_DEBUG
#define SYSY_IR_STRICT_CHECK 1
#define SYSY_IR_DEBUG 1
#endif


namespace SysYust::IR {

    inline constexpr bool strict_check_flag =
        #ifdef SYSY_IR_STRICT_CHECK
                    true;
        #else
                    false;
        #endif
    inline constexpr bool ir_debug_flag =
        #ifdef SYSY_IR_DEBUG
                true;
        #else
                false;
        #endif

    namespace alias {
        using i32 = std::int32_t;
        using u32 = std::uint32_t;
        using f32 = float;
    } // alias

    enum class symbol_type {
        func,
        v,
        im,
    };

    using namespace alias;

    /**
     * @brief 函数符号
     */
    struct func_symbol {
        using label_type = std::string;

        [[nodiscard]] label_type full() const;

        friend auto operator<=> (const func_symbol &lhs, const func_symbol &rhs) = default;

        const label_type symbol;
    };

    /**
     * @brief 变量标识符的类型
     */
    struct var_symbol {
        using label_type = std::string;
        static constexpr char seq = '@';

        [[nodiscard]] label_type full() const;
        friend auto operator<=> (const var_symbol&, const var_symbol &) = default;

        auto operator| (auto F) const {
            if constexpr (std::is_invocable_v<decltype(F), label_type, size_t>) {
                return std::invoke(F, symbol, revision);
            } else {
                return std::invoke(F, *this);
            }
        }

        const label_type symbol{};
        const std::size_t revision = 0;
        const Type * const type;
    };

    /**
     * @brief 常量标签
     */
    struct im_symbol {
        using data_type = std::variant<i32, f32>;
        using label_type = std::string;

        im_symbol(i32 val);
        im_symbol(f32 val);
        ~im_symbol() = default;

        [[nodiscard]] label_type full() const;
        auto operator| (auto F) const {
            return std::visit(F, data);
        }

        const Type * const type;
        const data_type data;
    };

    struct operant {
        using label_type = std::string;
        using data_type = std::variant<var_symbol, im_symbol>;

        operant(var_symbol val);
        operant(im_symbol im_val);

        auto operator| (auto F) const {
            return std::visit(F, symbol);
        }

        [[nodiscard]] label_type full() const;

        const symbol_type symbolType;
        const Type * const &type;
        const std::variant<var_symbol, im_symbol> symbol;
    };

    enum class compose_type {
        rr,
        ri,
        ii,
    };

    compose_type operant_compose(const operant &lhs, const operant &rhs);

    /**
     * @brief 将标签类型转换
     */
     template <typename TargetType>
     struct as_fn {
         as_fn() = default;
         TargetType operator() (auto &&i) {
             if constexpr (std::is_constructible_v<TargetType, decltype(i)>) {
                 return TargetType(i);
             } else {
                 return i | [](auto i) {return i;};
             }
         }
     };

     template <typename TargetType>
     constinit as_fn<TargetType> as; ///< 便利调用对象

     /**
      * @brief 函数信息的描述是结构
      */
     struct func_info {
         using side_effect_flag = bool;
         const Type *type = nullptr; ///< 函数的返回类型标识
         side_effect_flag side_effect = false; ///< 函数的副作用描述，待定
     };

} // IR

// func_symbol 的哈希对象
template<>
struct std::hash<SysYust::IR::func_symbol> {
    using key_type = SysYust::IR::func_symbol;
    std::size_t operator() (const key_type &e) const {
        return std::hash<key_type::label_type>{}(e.full());
    }
};

// val_symbol 哈希对象
template<>
struct std::hash<SysYust::IR::var_symbol> {
    using key_type = SysYust::IR::var_symbol;
    std::size_t operator() (const key_type &e) const {
        return symbol_hash(e.symbol) ^ revision_hash(e.revision);
    }
    std::hash<key_type::label_type> symbol_hash{};
    std::hash<std::size_t> revision_hash{};
};

// im_symbol 哈希对象
template<>
struct std::hash<SysYust::IR::im_symbol> {
    using key_type = SysYust::IR::im_symbol;
    std::size_t operator() (const key_type &e) const {
        return v_hash(e.data);
    }
    std::hash<SysYust::IR::im_symbol::data_type> v_hash;
};

// operant 哈希对象
template<>
struct std::hash<SysYust::IR::operant> {
    using key_type = SysYust::IR::operant;
    std::size_t operator() (const key_type &e) const {
        return v_hash(e.symbol);
    }
    std::hash<SysYust::IR::operant::data_type> v_hash;
};

#endif //SYSYUST_SYMBOLUTIL_H
