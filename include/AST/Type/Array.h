/// @file 类型标识 Array 类的实现

#ifndef SYSYUST_AST_ARRAY_H
#define SYSYUST_AST_ARRAY_H

#include <string>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <memory>

#include "TypeBase.h"

namespace SysYust::AST {

    /**
     * @brief 类型标识 Array
     * @details Array 中存储一个类型标识和一个标识数组维度信息的数组，内层维度的长度信息存储在数组的低位。比如 `int [1][2][3]`,
     * Array内将以 `[3,2,1]` 顺序存储三个数.
     */
    class Array: public TypeBase<Array> {
    public:
        /**
         * @brief 通过一个编译时不可知的类型标识构造 Array.
         * @param baseTypeArg 基类型,必须为 Int 或 Float ,要求在函数外检查.
         */
        Array(const Type &baseTypeArg, std::vector<std::size_t> dimension);

        /**
         * @brief 使用一个基类型和维度初始化列表构造 Array
         * @tparam T 基类型必须为 Int 或 Float 中的一个
         */
        template<typename T, std::enable_if_t<TypeTrait<T>::isBasicType, bool> = true >
        Array(const T &baseType, std::vector<std::size_t> dimensions)
        : _baseType(baseType)
        , _dimensions(std::move(dimensions)) {

        }

        /**
         * @brief 构造数组的数组
         * @param baseType Array 类型的参数，它的基类型将成为当前对象的基类型
         * @param dimensions 表达维数的初始化列表，它将和 baseType 的维度串联表示当前对象的维度
         */
        Array(const Array &baseType, const std::vector<std::size_t>& dimensions);

        /**
         * @brief 构建 Array 指针的静态方法
         */
        template<typename BaseType>
        static const Array& create(const BaseType& baseType, std::vector<std::size_t> d) {
            Array target(baseType, d);
            auto [rt, state] = _pool.insert(target);
            return *rt;
        }

        /**
         * @brief 获取当前数组对象的基类型
         */
        [[nodiscard]] const Type& baseType() const;

        /**
         * @brief 获取当前数组对象的维度列表
         */
        [[nodiscard]] const std::vector<std::size_t>& getDimension() const;

        /**
         * @brief 获取当前数组的维数
         */
        [[nodiscard]] std::size_t getRank() const;

        /**
         * @brief 获取指定维度的长度
         */
        [[nodiscard]] std::size_t getExtent(std::size_t) const;

        /**
         * @brief 解引用 layer 层后的类型
         */
        [[nodiscard]] const Type& index(std::size_t layer) const;

        /**
         * @brief 使用给定索引后产生的偏移量,外层数组的索引位于前
         */
        [[nodiscard]] std::size_t offsetWith(const std::vector<std::size_t> &ind) const;

        [[nodiscard]] bool match(const SysYust::AST::Array &) const override;
        [[nodiscard]] bool match(const SysYust::AST::Pointer &) const override;

        /**
         * @brief 定义了一种偏序，用于池
         */
        friend bool operator< (const Array& lhs, const Array &rhs) {
            if (lhs._baseType.type() != rhs._baseType.type()) {
                return lhs._baseType.type() < rhs._baseType.type();
            } else {
                return lhs._dimensions < rhs._dimensions;
            }
        }

        [[nodiscard]] std::string toString() const noexcept override;

    private:
        const Type &_baseType; ///< 数组的元素类型
        const std::vector<std::size_t> _dimensions; ///< 数组的维度,内层维度在低位。
        static std::set<Array> _pool;
    };

} // AST

#endif //SYSYUST_AST_ARRAY_H