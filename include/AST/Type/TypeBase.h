/// @file 声明语法树类型标识的基类和 CRTP 基类，并创建相关的变量模板
/// 所有类型标识通过 const T& 或 const T*的形式引用。

#ifndef SYSYUST_AST_TYPEBASE_H
#define SYSYUST_AST_TYPEBASE_H

#include <type_traits>
#include <string>
#include <unordered_set>

#include "utility/expected.h"

namespace SysYust::AST {

    /**
     * @brief 类型的枚举标识
     */
    enum class TypeId {
        Invalid = 0,
        Int,
        Float,
        Array,
        Pointer,
        Void,
        Function,
    };

    /**
     * @brief 获取类型绑定的TypeId,当不存在时为 TypeId::Invalid.类型标识子类型应当特化该变量模板。
     * @tparam T 类型标识类型
     */
    template<typename T>
    constexpr TypeId getTypeIdOf = TypeId::Invalid;

    // 类型标识的前置声明
    class Type;
    class Int;
    class Float;
    class Array;
    class Pointer;
    class Void;
    class Function;

    // getTypeIdOf的特化
    template<> constexpr TypeId getTypeIdOf<Int> = TypeId::Int;
    template<> constexpr TypeId getTypeIdOf<Float> = TypeId::Float;
    template<> constexpr TypeId getTypeIdOf<Array> = TypeId::Array;
    template<> constexpr TypeId getTypeIdOf<Pointer> = TypeId::Pointer;
    template<> constexpr TypeId getTypeIdOf<Void> = TypeId::Void;
    template<> constexpr TypeId getTypeIdOf<Function> = TypeId::Function;

    /**
     * @brief 类型标识特性,依赖于 getTypeIdOf 获取 TypeId 并据此计算
     * @tparam T 类型标识类型
     */
    template<typename T>
    struct TypeTrait {
        constexpr static TypeId Id = getTypeIdOf<T>;
        constexpr static bool isBasicType = Id == TypeId::Int || Id == TypeId::Float;
        constexpr static bool isArrayedType = isBasicType || Id == TypeId::Array;
        constexpr static bool isPointedType = isArrayedType;
        constexpr static bool isParamType = isBasicType || Id == TypeId::Pointer;
        constexpr static bool isReturnedType = isBasicType || Id == TypeId::Void;
        constexpr static bool isVarType = isBasicType || Id == TypeId::Array || Id == TypeId::Pointer;
    };

    /**
     * @brief 表示有效类型的标识的概念
     */
    template<typename T>
    concept ValidType = getTypeIdOf<std::remove_cvref_t<T>> != TypeId::Invalid;

    /**
     * @brief 通过类型标识的内容进行相等性判断
     */
    bool match(const Type &lhs, const Type &rhs);

    /**
     * @brief 类型标识的纯虚基类
     * @details 声明了所有类型归类的虚方法。
     */
    class Type {
    public:
        virtual ~Type() = default;

        // 获取 类型的ID
        [[nodiscard]] virtual TypeId  type() const = 0;

        // 类型归类
        [[nodiscard]] virtual bool isBasicType() const = 0;
        [[nodiscard]] virtual bool isArrayedType() const = 0;
        [[nodiscard]] virtual bool isPointedType() const = 0;
        [[nodiscard]] virtual bool isParamType() const = 0;
        [[nodiscard]] virtual bool isReturnedType() const = 0;
        [[nodiscard]] virtual bool isVarType() const = 0;

        // 以参数传递的标准进行匹配检测。
        [[nodiscard]] virtual bool match(const Type &) const = 0;
        [[nodiscard]] virtual bool match(const Int &) const {
            return false;
        }
        [[nodiscard]] virtual bool match(const Float &) const {
            return false;
        }
        [[nodiscard]] virtual bool match(const Array &) const {
            return false;
        }
        [[nodiscard]] virtual bool match(const Pointer &) const {
            return false;
        }
        [[nodiscard]] virtual bool match(const Void &) const {
            return false;
        }
        [[nodiscard]] virtual bool match(const Function &) const {
            return false;
        }

        /**
         * @brief 相等运算，匹配方式与 match 相同
         */
        friend bool operator== (const Type &lhs,const Type &rhs) {
            return AST::match(lhs, rhs);
        }

        /**
         * @brief 获取一个人类可读的类型字符串
         */
        [[nodiscard]] virtual std::string toString() const noexcept = 0;

    };

    /**
     * @brief Type的CRTP基类，为归类和类型获取提供默认实现
     * @tparam D 使用TypeBase的派生类
     * @todo 序列化和调试的字符串化
     */
    template<typename D>
    class TypeBase : public Type {
    public:
        ~TypeBase() override = default;

        [[nodiscard]] TypeId type() const final {
            return getTypeIdOf<D>;
        }

        [[nodiscard]] bool isBasicType() const final {
            return TypeTrait<D>::isBasicType;
        }
        [[nodiscard]] bool isArrayedType() const final {
            return TypeTrait<D>::isArrayedType;
        }
        [[nodiscard]] bool isPointedType() const final {
            return TypeTrait<D>::isArrayedType;
        }
        [[nodiscard]] bool isParamType() const final {
            return TypeTrait<D>::isParamType;
        }
        [[nodiscard]] bool isReturnedType() const final {
            return TypeTrait<D>::isParamType;
        };
        [[nodiscard]] bool isVarType() const final {
            return TypeTrait<D>::isVarType;
        }

        [[nodiscard]] bool match(const SysYust::AST::Type &rhs) const final {
            return rhs.match(static_cast<const D&>(*this));
        }
        [[nodiscard]] bool match(const D &) const override {
            return true;
        }

    };


} // AST

#endif //SYSYUST_AST_TYPEBASE_H
