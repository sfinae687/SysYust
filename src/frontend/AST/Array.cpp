/// @file 类型标识 Array 类的定义。

#include <cassert>

#include "AST/Array.h"
#include "AST/Pointer.h"

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

    Array::Array(const Type &baseType, std::initializer_list<std::size_t> dimension)
    : _baseType(baseType)
    , _dimensions(dimension) {
        assert(baseType.type() == TypeId::Int || baseType.type() == TypeId::Float);
    }

    Array::Array(const Array &baseType, std::initializer_list<std::size_t> dimensions)
    : _baseType(baseType._baseType)
    , _dimensions(concat(baseType._dimensions, dimensions)) {

    }

    const Type &Array::getType() const {
        return _baseType;
    }

    const std::vector<std::size_t> &Array::getDimension() const {
        return _dimensions;
    }

    bool Array::match(const Array &rhs) const {
        return _baseType.match(rhs._baseType) && _dimensions == rhs._dimensions;
    }

    bool Array::match(const Pointer &rhs) const {
        auto &pointed = rhs.getBase();
        if (pointed.type() != TypeId::Array) {
            return pointed.match(getType());
        } else { // 指针指向一个数组类型的情况
            assert(pointed.type() == TypeId::Array);

            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast) 经过验证的,所以使用静态转换
            auto &pointedArray = static_cast<const Array&>(pointed);

            // 执行比较
            if (pointedArray.getType().match(getType())) {
                auto &dl = getDimension();
                auto &dr = pointedArray.getDimension();
                return std::equal(dr.begin(), dr.end(), dl.begin());
            } else {
                return false; // 基类型不同，直接返回 false。
            }
        }
    }

    std::size_t Array::getRank() const {
        return _dimensions.size();
    }

    std::size_t Array::getExtent(std::size_t i) const {
        return _dimensions[i];
    }


} // AST