/// @file 类型标识 Array 类的实现

#ifndef SYSYUST_AST_ARRAY_H
#define SYSYUST_AST_ARRAY_H

#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "Type.h"

namespace SysYust::AST {

    /**
     * @brief 用于数组于指针之间判断相容性的比较运算符。
     */
    bool operator== (const Array&, const Pointer&);

    /**
     * @brief 类型标识 Array
     */
    class Array: public TypeBase<Array> {
        friend bool operator== (const Array&, const Pointer&);
    public:
        /**
         * @brief 使用一个基类型和维度初始化列表构造 Array
         * @tparam T 基类型必须为 Int 或 Float 中的一个
         */
        template<typename T, std::enable_if_t<getTypeIdOf<T> == TypeId::Int || getTypeIdOf<T> == TypeId::Float> >
        Array(const T &baseType, std::initializer_list<std::size_t > dimensions)
                : _baseType(baseType)
                , _dimensions(dimensions) {

        }

        /**
         * @brief 构造数组的数组
         * @param baseType Array 类型的参数，它的基类型将成为当前对象的基类型
         * @param dimensions 表达维数的初始化列表，它将和 baseType 的维度串联表示当前对象的维度
         */
        Array(const Array &baseType, std::initializer_list<std::size_t > dimensions);

        /**
         * @brief 获取当前数组对象的基类型
         */
        const Type& getType();
        /**
         * @brief 获取当前数组对象的维度
         */
        const std::vector<std::size_t>& getDimension();

        [[nodiscard]] bool match(const SysYust::AST::Array &) const override;

    private:
        const Type &_baseType; ///< 数组的元素类型
        const std::vector<std::size_t> _dimensions; ///< 数组的维度,内层维度在低位。
    };

} // AST

#endif //SYSYUST_AST_ARRAY_H
