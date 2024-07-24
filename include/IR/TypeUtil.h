//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_TYPEUTIL_H
#define SYSYUST_TYPEUTIL_H

#include <utility>
#include <variant>
#include <vector>
#include <memory>
#include <optional>
#include <set>

namespace SysYust::IR {

    /**
     * @brief 类型标识类型，使用 const Type*, const Type &引用
     */
    class Type {
        friend struct std::hash<Type>;
    public:
        static constexpr std::size_t int_size = 4;
        static constexpr std::size_t float_size = 4;
        static constexpr std::size_t pointer_size = 8;
        enum TypeId {
            i,
            f,
            arr,
            ptr,
        };


        /**
         * @brief 获取 Type 单例的工厂方法
         */
        static const Type *get(Type::TypeId id, const Type *ptr = nullptr, unsigned int data = 0);
        Type(const Type&) = default;
        ~Type() = default;

        friend auto operator<=> (const Type &, const Type &) = default;

        [[nodiscard]] bool isInt() const {
            return id == i;
        }
        [[nodiscard]] bool isFlt() const {
            return id == f;
        }
        [[nodiscard]] bool isArr() const {
            return id == arr;
        }
        [[nodiscard]] bool isPtr() const {
            return id == ptr;
        }
        [[nodiscard]] bool isScalar() const {
            return isInt() || isFlt();
        }
        [[nodiscard]] bool isPlain() const {
            return isInt() || isFlt() || isPtr();
        }

        /**
         * @brief 获取类型单元的存储大小，单位为 1 byte
         */
        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] const Type* getSubType() const {
            return subType;
        }

        [[nodiscard]] std::size_t getData() const {
            return data;
        }

    private:

        Type(TypeId id, unsigned data, const Type *ptr)
            : id(id)
            , data(data)
            , subType(ptr)
        {

        }

        static std::set<Type> _singletons;

        TypeId id : 16;
        std::size_t data : 48 = 0;
        const Type *subType = nullptr;
    };


} // IR

template<>
struct std::hash<SysYust::IR::Type> {
    using value_type = SysYust::IR::Type;
    std::size_t operator() (const value_type &v) {
        auto val = (v.id << 24) | v.data;
        auto ptr = v.subType;
        return std::hash<decltype(val)>{}(val) ^ std::hash<decltype(ptr)>{}(ptr);
    }
};

#endif //SYSYUST_TYPEUTIL_H
