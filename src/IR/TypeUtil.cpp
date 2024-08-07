//
// Created by LL06p on 24-7-20.
//

#include "IR/TypeUtil.h"


namespace SysYust::IR {

    std::set<Type> Type::_singletons{};

    /**
     * Type::get(arr,
     */
    const Type *Type::get(Type::TypeId id, const Type *ptr, unsigned int data) {
        Type toBuild(id, data, ptr);
        if (!_singletons.contains(toBuild)) {
            _singletons.insert(toBuild);
        }
        return &*_singletons.find(toBuild);
    }

    std::size_t Type::size() const {
        if (isInt()) {
            return int_size;
        } else if (isFlt()) {
            return float_size;
        } else if (isPtr()) {
            return pointer_size;
        } else if (isArr()) {
            return _data * _subType->size();
        } else {
            __builtin_unreachable();
        }
    }

    Type::TypeId Type::id() const {
        return _id;
    }

    const Type *Type::root_type() const {
        if (isBasic()) {
            return this;
        } else {
            return subtype();
        }
    }
} // IR