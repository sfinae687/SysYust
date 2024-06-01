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
     * @details 额外定义一个运算符用于处理 Pointer 与 Array 比较的参数顺序问题。Array 和 Pointer 的 对另一个类型的 match 操作将委托到该
     * 函数上。
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
        template<typename T, std::enable_if_t<TypeTrait<T>::isBasicType, bool> = true >
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
        [[nodiscard]] const Type& getType() const;
        /**
         * @brief 获取当前数组对象的维度
         */
        [[nodiscard]] const std::vector<std::size_t>& getDimension() const;

        [[nodiscard]] bool match(const SysYust::AST::Array &) const override;
        [[nodiscard]] bool match(const SysYust::AST::Pointer &) const override;

    private:
        const Type &_baseType; ///< 数组的元素类型
        const std::vector<std::size_t> _dimensions; ///< 数组的维度,内层维度在低位。
    };

    /// @todo 添加 Array 的 makeType 特化，使得同一种 Array 指向同一个实例。

} // AST

#endif //SYSYUST_AST_ARRAY_H
