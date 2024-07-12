#include "Interpreter/Value.h"

#include <cassert>

#include "AST/Type/TypeBase.h"
#include "fmt/core.h"

namespace SysYust::AST::Interpreter {

Value Value::toLValue() {
    assert(isUndef() && "You can't use an undef value!");
    assert(!is_lvalue);
    ValueType ret;
    switch (type->type()) {
        case TypeId::Int:
            ret = &std::get<Int_t>(_value);
            break;
        case TypeId::Float:
            ret = &std::get<Float_t>(_value);
            break;
        case TypeId::Array:
        case TypeId::Pointer:
            ret = _value;
        default:
            assert(false && "blabla");
            break;
    }
    return {type, ret, false};
}

Value Value::toRValue() {
    assert(isUndef() && "You can't use an undef value!");
    assert(is_lvalue);
    ValueType ret;
    switch (type->type()) {
        case TypeId::Int:
            ret = *std::get<Int_t *>(_value);
            break;
        case TypeId::Float:
            ret = *std::get<Float_t *>(_value);
            break;
        case TypeId::Array:
        case TypeId::Pointer:
            ret = _value;
        default:
            assert(false && "blabla");
            break;
    }
    return {type, ret, false};
}

Value Value::assign(Value rv) {
    assert(is_lvalue);
    assert(type == rv.type);
    switch (type->type()) {
        case TypeId::Int:
            get<Int>() = get<Int>();
            break;
        case TypeId::Float:
            get<Float>() = get<Float>();
            break;
        default:
            assert("SysY spec only allow basic type assign.");
    }
    return *this;
}

std::string Value::toString() noexcept {
    std::string ret;
    switch (type->type()) {
        case TypeId::Int:
            ret = fmt::format("(Int {})", get<Int>());
            break;
        case TypeId::Float:
            ret = fmt::format("(Float {})", get<Float>());
            break;
        case TypeId::Array:
            ret = fmt::format("({} {})", type->toString(), "#array_val");
            break;
        case TypeId::Pointer:
            ret = fmt::format("({} {})", type->toString(), "#pointer_val");
            break;
        default:
            assert(0 && "Invalid Value");
    }
    if (is_lvalue) ret = fmt::format("&{}", ret);
    return ret;
}

// std::string Value::printArray() {
//     auto *arr = dynamic_cast<const Array *>(type);
//     auto &dim = arr->getDimension();
//     if (arr->baseType() == Int_v) {

//     } else {

//     }
// }

}  // namespace SysYust::AST::Interpreter