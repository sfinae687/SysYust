//
// Created by LL06p on 24-7-20.
//

#include "IR/TypeUtil.h"


namespace SysYust::IR {

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
            return data * subType->size();
        } else {
            __builtin_unreachable();
        }
    }
} // IR