/// @file 类型标识 Array 类的定义。

#include "AST/Array.h"

namespace SysYust::AST {
    namespace {
        /**
         * @brief 将两个中 vector 串联
         */
        std::vector<std::size_t> concat(const std::vector<std::size_t> &lhs, const std::vector<std::size_t> &rhs) {
            using std::begin;
            using std::end;
            std::vector<std::size_t> rt(lhs.size() + rhs.size());
            std::copy(begin(lhs), end(lhs), begin(rt));
            std::copy(begin(rhs), end(rhs), begin(rt)+lhs.size());
            return rt;
        }
    }

    Array::Array(const Array &baseType, std::initializer_list<std::size_t> dimensions)
    : _baseType(baseType._baseType)
    , _dimensions(concat(baseType._dimensions, dimensions)) {

    }

    const Type &Array::getType() {
        return _baseType;
    }

    const std::vector<std::size_t> &Array::getDimension() {
        return _dimensions;
    }

    bool Array::match(const Array &rhs) const {
        return _baseType == rhs._baseType && _dimensions == rhs._dimensions;
    }

    bool operator==(const Array &, const Pointer &) {
        /// @todo Pointer 和 Array 的相等性看比较函数
        return false;
    }
} // AST