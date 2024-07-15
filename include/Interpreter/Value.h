/// SysYust 解释器的"值"实现

#ifndef SYSYUST_INTERPRETER_VALUE_H
#define SYSYUST_INTERPRETER_VALUE_H

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <variant>

#include "AST/Type/Float.h"
#include "AST/Type/Int.h"
#include "AST/Type/TypeBase.h"
#include "AST/Type/Void.h"

namespace SysYust::AST::Interpreter {

template <bool A, class B, class C>
using if_t = std::conditional_t<A, B, C>;
template <class A, class B>
concept eq_v = std::is_same_v<A, B>;

class Value {
   public:
    using Int_t = std::int32_t;
    using Float_t = float;

    template <class T>
    using val_t =
        if_t<eq_v<T, Int>, Int_t, if_t<eq_v<T, Float>, Float_t, void>>;

    const Type *type;

    /// Int / Float -> int / float
    /// Array -> ptr (长度信息由 type 指定)
    /// Pointer -> ptr
    /// LValue Int / Float -> ptr
    /// LValue Array -> ptr
    /// LPointer -> ptr
    using ValueType = std::variant<Int_t, Float_t, Int_t *, Float_t *>;
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
    }

    Value toLValue();
    Value toRValue();

    template <class T>
    val_t<T> &get();

    bool isUndef() const {
        return type->type() == TypeId::Void;
    }

    Value assign(Value rv);

    std::string toString() noexcept;
};

template <class T>
Value::val_t<T> &Value::get() {
    assert(type != &Void_v && "You can't use an undef value!");
    assert(type->type() == getTypeIdOf<T>);
    static_assert(TypeTrait<T>::isBasicType &&
                  "Only direct Int / Float value can be used");
    return is_lvalue ? *std::get<val_t<T> *>(_value)
                     : std::get<val_t<T>>(_value);
}

}  // namespace SysYust::AST::Interpreter

#endif  // SYSYUST_INTERPRETER_VALUE_H