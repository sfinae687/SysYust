/// SysYust 解释器的"值"实现

#ifndef SYSYUST_INTERPRETER_VALUE_H
#define SYSYUST_INTERPRETER_VALUE_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <sstream>
#include <type_traits>
#include <variant>
#include <vector>

#include "AST/Type.h"
#include "AST/Type/TypeBase.h"
// #include "Interpreter/Interpreter.h"
#include "Logger.h"

namespace SysYust::AST::Interpreter {

template <bool A, class B, class C>
using if_t = std::conditional_t<A, B, C>;
template <class A, class B>
concept eq_v = std::is_same_v<A, B>;

class Memory {
   private:
    using Int_t = std::int32_t;
    using Float_t = float;
    using DataType = std::variant<std::vector<Int_t>, std::vector<Float_t>>;

    int size;
    void *data;

    Memory(const Type &type) {
        switch (type.type()) {
            case TypeId::Int:
                size = 1;
                data = new int();
                break;
            case TypeId::Float:
                size = 1;
                data = new float();
                break;
            case TypeId::Array: {
                auto &arr_type =
                    *dynamic_cast<Array *>(&const_cast<Type &>(type));
                auto &dim = arr_type.getDimension();
                long long prod = 1;
                for (auto i : dim) {
                    prod *= i;
                    assert(i >= 0 && "Negtive array addr");
                }
                assert(prod > 0 && "Unimpl zero array");
                data = new std::int32_t[prod]();
                size = prod;
                break;
            }
            case TypeId::Pointer:
                assert(false && "Pointer only can be used in ParamDecl");
                break;
            default:
                assert(false && "This type can't allocate memory");
        }
        LOG_WARN("Memory create {}", *((int *)data));
    }

    friend class MemorySlice;

   public:
    ~Memory() {
        assert(data);
        LOG_WARN("Memory destruct");
        delete[] static_cast<std::int32_t *>(data);
    }
};

class MemorySlice {
   public:
    using Int_t = std::int32_t;
    using Float_t = float;

    template <typename T>
    using val_t =
        if_t<eq_v<T, Int>, Int_t, if_t<eq_v<T, Float>, Float_t, void>>;

    std::shared_ptr<Memory> ptr;
    void *pos;
    int len;

    MemorySlice(const Type &type) : ptr(new Memory(type)) {
        pos = ptr->data;
        len = ptr->size;
    }

    MemorySlice(std::shared_ptr<Memory> ptr, void *pos, int len)
        : ptr(ptr), pos(pos), len(len) {
    }

    MemorySlice subSlice(void *pos, int len) {
        return MemorySlice(ptr, pos, len);
    }

    template <typename T>
    val_t<T> *access() {
        assert(TypeTrait<T>::isBasicType);
        return static_cast<val_t<T> *>(pos);
    }
};

class Value {
   public:
    using Int_t = std::int32_t;
    using Float_t = float;

    template <class T>
    using val_t =
        if_t<eq_v<T, Int>, Int_t, if_t<eq_v<T, Float>, Float_t, void>>;

    const Type *type;

    /// Int / Float -> int / float
    /// Array -> MemorySlice (长度信息由 type 指定)
    /// LValue / Pointer -> MemorySlice
    using ValueType = std::variant<Int_t, Float_t, MemorySlice>;
    ValueType _value;

    bool is_lvalue;

    /// Dummy / Undef Value type = Void
    Value() : type(&Void_v), _value(0), is_lvalue(false) {
    }

    /**
     * @brief Construct a new Value object
     *
     * @param type_ 类型
     * @param _value_ 值
     * @param is_lvalue_ 是否是左值（默认为 0）
     */
    Value(const Type *type_, ValueType _value_, bool is_lvalue_ = false)
        : type(type_), is_lvalue(is_lvalue_), _value(_value_) {
        assert(type->isBasicType() && (is_lvalue == (_value.index() == 2)) ||
               !type->isBasicType() && _value.index() == 2);
    }

    Value toLValue();
    Value toRValue();

    template <class T>
    val_t<T> get();

    template <class T>
    val_t<T> &getRef();

    void *getPtr();

    bool isUndef() const {
        return type->type() == TypeId::Void;
    }

    bool isLValue() const {
        return is_lvalue;
    }

    Value assign(Value rv);

    std::string toString() noexcept;
    std::string printMem();
};

template <class T>
Value::val_t<T> Value::get() {
    assert(type != &Void_v && "You can't use an undef value!");
    assert(type->type() == getTypeIdOf<T>);
    static_assert(TypeTrait<T>::isBasicType &&
                  "Only direct Int / Float value can be used");
    if (is_lvalue) {
        auto &mslice = std::get<MemorySlice>(_value);
        return *mslice.access<T>();
    } else {
        return std::get<val_t<T>>(_value);
    }
}

template <typename T>
Value::val_t<T> &Value::getRef() {
    assert(is_lvalue && "you can't get a ref of a RValue");
    assert(type->type() == getTypeIdOf<T>);
    static_assert(TypeTrait<T>::isBasicType &&
                  "Only direct Int / Float value can be used");
    auto &mslice = std::get<MemorySlice>(_value);
    return *mslice.access<T>();
}

}  // namespace SysYust::AST::Interpreter

#endif  // SYSYUST_INTERPRETER_VALUE_H