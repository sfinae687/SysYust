/// @file
/// Pointer 类型标识的定义

#ifndef SYSYUST_AST_POINTER_H
#define SYSYUST_AST_POINTER_H

#include <set>
#include <vector>

#include "TypeBase.h"
#include "Array.h"

namespace SysYust::AST {

    class Pointer: public TypeBase<Pointer> {
    public:
        /**
         * @brief 通过编译时不可知的基类型设置
         * @param baseType 基类型,为 Int, Float, Array 之一, 由主调检查.
         */
        explicit Pointer(const Type &baseType);

        /**
         * @brief 通过编译时可知的基类型设置 Pointer
         */
        template<typename T, std::enable_if_t<TypeTrait<T>::isPointedType, bool> = true >
        explicit Pointer(const T &baseType)
        : _baseType(baseType) {

        }

        /**
         * @brief 构建一个 Pointer，带有单例池
         */
        static const Pointer& create(const Type &baseType) {
            Pointer target(baseType);
            auto [rt, state] = _pool.insert(target);
            return *rt;
        }

        /**
         * @brief 获取指向的类型
         * @todo 重构为 baseType
         */
        [[nodiscard]] const Type& getBase() const;

        /**
         * @brief 解引用 layer 层后的类型
         */
        [[nodiscard]] const Type& index(std::size_t layer) const;

        /**
         * @brief 使用给定索引后产生的偏移量,外层数组的索引位于前
         */
        [[nodiscard]] std::size_t offsetWith(const std::vector<std::size_t> &ind) const;



        [[nodiscard]] bool match(const SysYust::AST::Pointer &) const override;
        [[nodiscard]] bool match(const SysYust::AST::Array &) const override;

        /**
         * @brief 小于比较运算
         */
        friend bool operator< (const Pointer &lhs, const Pointer &rhs) {
            auto lt = lhs._baseType.type();
            auto rt = rhs._baseType.type();
            if (lt == TypeId::Array && lt == rt) {
                auto lA = static_cast<const Array&>(lhs._baseType);
                auto rA = static_cast<const Array&>(rhs._baseType);
                return lA < rA;
            } else {
                return lhs._baseType.type() < rhs._baseType.type();
            }
        }

        [[nodiscard]] std::string toString() const noexcept override;

    private:
        const Type &_baseType; ///< 基类型，为Int，Float，Array之一
        static std::set<Pointer> _pool;
    };

} // AST

#endif //SYSYUST_AST_POINTER_H
