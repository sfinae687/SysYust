/// @file Pointer 类型标识的定义

#ifndef SYSYUST_AST_POINTER_H
#define SYSYUST_AST_POINTER_H

#include "AST/TypeBase.h"
#include "AST/Array.h"

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
         * @brief 获取指向的类型
         */
        [[nodiscard]] const Type& getBase() const;

        [[nodiscard]] bool match(const SysYust::AST::Pointer &) const override;
        [[nodiscard]] bool match(const SysYust::AST::Array &) const override;
    private:
        const Type &_baseType; ///< 基类型，为Int，Float，Array之一
    };

} // AST

#endif //SYSYUST_AST_POINTER_H
