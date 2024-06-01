/// @file 声明语法树类型标识的基类和 CRTP 基类，并创建相关的变量模板

#ifndef SYSYUST_AST_TYPE_H
#define SYSYUST_AST_TYPE_H

#include <type_traits>
#include <string>

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
     * @brief 类型标识的纯虚基类
     * @details 声明了所有类型归类的虚方法。
     */
    class Type {
    public:
        virtual ~Type() = default;

        // 获取 类型的ID
        virtual TypeId  type() = 0;

        // 类型归类
        virtual bool isBasicType() = 0;
        virtual bool isArrayedType() = 0;
        virtual bool isPointedType() = 0;
        virtual bool isParamType() = 0;
        virtual bool isReturnedType() = 0;
        virtual bool isVarType() = 0;

        // 比较处理
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

        friend bool operator== (const Type &lhs, const Type &rhs) {
            return lhs.match(rhs);
        }
    };

    /**
     * @brief Type的CRTP基类，为归类和类型获取提供默认实现
     * @tparam D 使用TypeBase的派生类
     */
    template<typename D>
    class TypeBase : public Type {
    public:
        ~TypeBase() override = default;

        TypeId type() final {
            return getTypeIdOf<D>;
        }

        bool isBasicType() final {
            return TypeTrait<D>::isBasicType;
        }
        bool isArrayedType() final {
            return TypeTrait<D>::isArrayedType;
        }
        bool isPointedType() final {
            return TypeTrait<D>::isArrayedType;
        }
        bool isParamType() final {
            return TypeTrait<D>::isParamType;
        }
        bool isReturnedType() final {
            return TypeTrait<D>::isParamType;
        };
        bool isVarType() final {
            return TypeTrait<D>::isVarType;
        }

        [[nodiscard]] bool match(const SysYust::AST::Type &rhs) const final {
            return rhs.match(static_cast<const D&>(*this));
        }
    };

    /**
     * @brief 构造 Type 的子对象
     * @todo 添加错误处理
     */
    template<typename T, typename... Args>
    [[maybe_unused]] expected<std::enable_if_t< getTypeIdOf<T> != TypeId::Invalid && std::is_constructible_v<T, Args...>, T>, std::string>
    makeType(Args&&... args) {
        return T(std::forward<Args>(args)...);
    }

} // AST

#endif //SYSYUST_AST_TYPE_H
