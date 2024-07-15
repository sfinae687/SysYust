/// @file Pointer 类型标识的实现

#include <cassert>

#include "AST/Type/Pointer.h"

namespace SysYust::AST {
    Pointer::Pointer(const Type &baseType)
    : _baseType(baseType) {
        assert(baseType.type() == TypeId::Int || baseType.type() == TypeId::Float || baseType.type() == TypeId::Array);
    }

    bool Pointer::match(const Array &rhs) const {
        return rhs.match(*this); // 将 match 操作委托给 Array 中定义的 match。
    }

    bool Pointer::match(const Pointer &rhs) const {
        return _baseType.match(rhs._baseType);
    }

    const Type &Pointer::getBase() const {
        return _baseType;
    }

    std::set<Pointer> Pointer::_pool{};

    std::string Pointer::toString() const noexcept {
        return getBase().toString() + "*";
    }

    const Type &Pointer::index(std::size_t layer) const {
        if (layer == 0) {
            return *this;
        } else if (layer == 1) {
            return _baseType;
        } else {
            assert(getBase().type() == TypeId::Array);
            auto &pointedArray = static_cast<const Array&>(getBase());
            return pointedArray.index(layer-1);
        }
    }

    std::size_t Pointer::offsetWith(const std::span<std::size_t> &ind) const {
        assert(!ind.empty());
        if (getBase().type() == TypeId::Array) {
            auto &pointedArray = static_cast<const Array&>(getBase());
            auto &dim = pointedArray.getDimension();
            auto block = std::reduce(dim.begin(), dim.end(), 1ull, std::multiplies{});
            if (ind.size() > 1) {
                auto baseOffset = pointedArray.offsetWith({ind.begin()+1, ind.end()});
                return block * ind.front() + baseOffset;
            } else {
                return block * ind.front();
            }
        } else {
            assert(ind.size() == 1);
            return ind.front();
        }
    }
} // AST