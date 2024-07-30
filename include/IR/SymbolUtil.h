///@file Instruction 的辅助工具，主要为符号的实现

#ifndef SYSYUST_SYMBOLUTIL_H
#define SYSYUST_SYMBOLUTIL_H

#include <utility>
#include <type_traits>
#include <functional>
#include <variant>
#include <string>
#include <compare>

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

        label_type symbol;
    };

    /**
     * @brief 变量标识符的类型
     */
    struct var_symbol {
        using label_type = std::string;
        static constexpr char seq = '@';

        [[nodiscard]] label_type full() const;
        friend std::strong_ordering operator<=> (const var_symbol &lhs, const var_symbol &rhs) noexcept {
            if (auto key1 = lhs.symbol <=> rhs.symbol; key1 == 0) {
                return key1;
            } else {
                return lhs.revision <=> rhs.revision;
            }
        }
        friend bool operator== (const var_symbol &lhs, const var_symbol &rhs) {
            return lhs.symbol == rhs.symbol && lhs.revision == rhs.revision;
        }

        auto operator| (auto F) const {
            if constexpr (std::is_invocable_v<decltype(F), label_type, size_t>) {
                return std::invoke(F, symbol, revision);
            } else {
                return std::invoke(F, *this);
            }
        }

        label_type symbol{};
        std::size_t revision = 0;
        const Type * type = Type::get(Type::none);
    };

    // 这里需要 <compare>
    static_assert(std::three_way_comparable<var_symbol>);

    /**
     * @brief 常量标签
     */
    struct im_symbol {
        /**
         * @brief 标签，用于标识改立即数具有特殊的值.标签类型的立即数
         */
        enum flag {
            undef,
        };
        using data_type = std::variant<i32, f32, flag>;
        using label_type = std::string;

        im_symbol(i32 val);
        im_symbol(f32 val);
        im_symbol(flag f, const Type *type = Type::get(Type::i));
        ~im_symbol() = default;

        friend auto operator<=> (const im_symbol &lhs, const im_symbol &rhs) = default;

        [[nodiscard]] label_type full() const;
        auto operator| (auto F) const {
            return std::visit(F, data);
        }

        data_type data;
        const Type * type = Type::get(Type::none);
    };

    struct operant {
        using label_type = std::string;
        using data_type = std::variant<var_symbol, im_symbol>;

        operant(var_symbol val);
        operant(im_symbol im_val);
        operant(const operant &oth)
            : _symbolType(oth._symbolType)
            , _type(oth._type)
            , _symbol(oth._symbol)
        {
        }
        operant& operator= (const operant &oth) {
            if (this == &oth) {
                return *this;
            }
            _symbolType = oth._symbolType;
            _type = oth._type;
            _symbol = oth._symbol;
            return *this;
        }

        friend auto operator<=> (const operant &lhs, const operant &rhs) noexcept {
            if (auto key1 = lhs._symbolType <=> rhs._symbolType; key1 != 0) {
                return key1;
            } else if (auto key2 = lhs._type <=> rhs._type; key2 != 0) {
                return key2;
            } else {
                return lhs._type <=> rhs._type;
            }
        }
        friend bool operator== (const operant &lhs, const operant &rhs) {
            return lhs._symbolType == rhs._symbolType
            && lhs._type == rhs._type
            && lhs._symbol == rhs._symbol;
        }

        auto operator| (auto F) const {
            return std::visit(F, symbol);
        }

        [[nodiscard]] label_type full() const;

        [[nodiscard]] var_symbol var() const;
        [[nodiscard]] im_symbol im() const;

    private:
        symbol_type _symbolType;
        const Type *_type;
        std::variant<var_symbol, im_symbol> _symbol;
    public:
        const symbol_type &symbolType{_symbolType};
        const Type * const &type{_type};
        const std::variant<var_symbol, im_symbol> symbol{_symbol};
    };

    enum class compose_type {
        rr,
        ri,
        ii,
    };

    compose_type operant_compose(const operant &lhs, const operant &rhs);

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
