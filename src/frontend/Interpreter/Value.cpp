#include "Interpreter/Value.h"

#include <cassert>
#include <sstream>

#include "AST/Type/Float.h"
#include "AST/Type/TypeBase.h"
#include "Interpreter/Interpreter.h"
#include "fmt/core.h"

namespace SysYust::AST::Interpreter {

void *Value::getPtr() {
    assert(is_lvalue && "you can't get a ref of a RValue");
    auto mslice = std::get<MemorySlice>(_value);
    return mslice.pos;
}

Value Value::toLValue() {
    assert(!isUndef() && "You can't use an undef value!");
    ValueType ret;
    switch (type->type()) {
        case TypeId::Int: {
            MemorySlice mslice(Int_v);
            *mslice.access<Int>() = get<Int>();
            ret = mslice;
            break;
        }
        case TypeId::Float: {
            MemorySlice mslice(Float_v);
            *mslice.access<Float>() = get<Float>();
            ret = mslice;
            break;
        }
        case TypeId::Array:
        case TypeId::Pointer:
            ret = _value;
            break;
        default:
            assert(false && "RValTypeErr");
            break;
    }
    return {type, ret, true};
}

Value Value::toRValue() {
    assert(!isUndef() && "You can't use an undef value!");
    if (!is_lvalue) return *this;
    assert(is_lvalue);
    ValueType ret;
    switch (type->type()) {
        case TypeId::Int:
            ret = this->get<Int>();
            break;
        case TypeId::Float:
            ret = this->get<Float>();
            break;
        case TypeId::Array:
        case TypeId::Pointer:
            // copy MemorySlice
            ret = std::get<MemorySlice>(_value);
            break;
        default:
            assert(false && "Value type err");
            break;
    }
    return {type, ret, false};
}

Value Value::assign(Value rv) {
    assert(is_lvalue);
    assert(type == rv.type);
    assert(!rv.isLValue() && "rhs should be rvalue");
    assert(rv.type->isBasicType() && "SysY spec only allow basic type assign.");
    switch (type->type()) {
        case TypeId::Int:
            getRef<Int>() = rv.get<Int>();
            break;
        case TypeId::Float:
            getRef<Float>() = rv.get<Float>();
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
            ret = fmt::format("({} {})", type->toString(), printMem());
            break;
        case TypeId::Pointer:
            ret = fmt::format("({} {})", type->toString(), printMem());
            break;
        case TypeId::Void:
            ret = fmt::format("(Void)");
            break;
        default:
            assert(false && "Invalid Value.");
    }
    if (is_lvalue) ret = fmt::format("&{}", ret);
    return ret;
}

std::string Value::printMem() {
    if (type->type() == TypeId::Array) {
        auto &val_type = dynamic_cast<Array *>(const_cast<Type *>(type))->baseType();
        if (val_type.type() == TypeId::Int) {
            auto mslice = std::get<MemorySlice>(_value);
            auto pos = static_cast<Int_t *>(mslice.pos);
            auto len = mslice.len;
            auto upper = std::min(len, 10);
            std::stringstream ret;
            for (int i = 0; i < upper; ++i) {
                ret << pos[i] << (i == upper - 1 ? "" : ", ");
            }
            if (len > 10) {
                ret << "...";
            }
            return ret.str();
        } else {
            auto mslice = std::get<MemorySlice>(_value);
            auto pos = static_cast<Float_t *>(mslice.pos);
            auto len = mslice.len;
            auto upper = std::min(len, 10);
            std::stringstream ret;
            for (int i = 0; i < upper; ++i) {
                ret << pos[i] << (i == upper - 1 ? "" : ", ");
            }
            if (len > 10) {
                ret << "...";
            }
            return ret.str();
        }
    } else {
        assert(type->type() == TypeId::Pointer);
        auto &arr_type = dynamic_cast<Pointer *>(const_cast<Type *>(type))->getBase();
        auto &val_type = dynamic_cast<Array *>(const_cast<Type *>(&arr_type))->baseType();
        if (val_type.type() == TypeId::Int) {
            auto mslice = std::get<MemorySlice>(_value);
            auto pos = static_cast<Int_t *>(mslice.pos);
            auto len = mslice.len;
            auto upper = std::min(len, 10);
            std::stringstream ret;
            for (int i = 0; i < upper; ++i) {
                ret << pos[i] << (i == upper - 1 ? "" : ", ");
            }
            if (len > 10) {
                ret << "...";
            }
            return ret.str();
        } else {
            auto mslice = std::get<MemorySlice>(_value);
            auto pos = static_cast<Float_t *>(mslice.pos);
            auto len = mslice.len;
            auto upper = std::min(len, 10);
            std::stringstream ret;
            for (int i = 0; i < upper; ++i) {
                ret << pos[i] << (i == upper - 1 ? "" : ", ");
            }
            if (len > 10) {
                ret << "...";
            }
            return ret.str();
        }
    }
}

}  // namespace SysYust::AST::Interpreter