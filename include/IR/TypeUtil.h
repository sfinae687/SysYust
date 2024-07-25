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
            none = -1,
            i,
            f,
            arr,
            ptr,
            dyn, ///< 特殊标记，用于表示无法从指令类型推断参数
        };


        /**
         * @brief 获取 Type 单例的工厂方法
         */
        static const Type *get(Type::TypeId id, const Type *ptr = nullptr, unsigned int data = 0);
        Type(const Type&) = default;
        ~Type() = default;

        friend auto operator<=> (const Type &, const Type &) = default;

        [[nodiscard]] bool isInt() const {
            return _id == i;
        }
        [[nodiscard]] bool isFlt() const {
            return _id == f;
        }
        [[nodiscard]] bool isArr() const {
            return _id == arr;
        }
        [[nodiscard]] bool isPtr() const {
            return _id == ptr;
        }

        /**
         * @brief 不具有子类型的类型
         */
        [[nodiscard]] bool isBasic() const {
            return isInt() || isFlt();
        }

        /**
         * @brief 具有固定长度的类型
         */
        [[nodiscard]] bool isScalar() const {
            return isInt() || isFlt() || isPtr();
        }

        /**
         * @brief 获取类型单元的存储大小，单位为 1 byte
         */
        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] TypeId id() const;
        [[nodiscard]] const Type* subtype() const {
            return _subType;
        }
        [[nodiscard]] const Type* root_type() const;

        [[nodiscard]] std::size_t data() const {
            return _data;
        }

    private:

        Type(TypeId id, unsigned data, const Type *ptr)
            : _id(id)
            , _data(data)
            , _subType(ptr)
        {

        }

        static std::set<Type> _singletons;

        TypeId _id : 16;
        std::size_t _data : 48 = 0;
        const Type *_subType = nullptr;
    };


} // IR

template<>
struct std::hash<SysYust::IR::Type> {
    using value_type = SysYust::IR::Type;
    std::size_t operator() (const value_type &v) {
        auto val = (v._id << 24) | v._data;
        auto ptr = v._subType;
        return std::hash<decltype(val)>{}(val) ^ std::hash<decltype(ptr)>{}(ptr);
    }
};

#endif //SYSYUST_TYPEUTIL_H
