/// SysYust AST 的解释器，用于验证 前端 正确性，另给一点信心（

#ifndef SYSYUST_INTERPRETER_H
#define SYSYUST_INTERPRETER_H

#include <iostream>
#include <optional>
#include <stack>
#include <variant>

#include "AST/Env/SymbolTable.h"
#include "AST/Node/ArrayRef.h"
#include "AST/Node/BinaryOp.h"
#include "AST/Node/Compare.h"
#include "AST/Node/Continue.h"
#include "AST/Node/UnaryOp.h"
#include "AST/SyntaxTree.h"
#include "AST/Type/Void.h"
#include "Interpreter/Value.h"

// For Debug

class sp_t {
   public:
    int counter = 0;
    void push() {
        counter++;
    }
    void pop() {
        counter--;
    }

};

static sp_t sp;

std::ostream &operator<<(std::ostream &os, sp_t &sp);

#define println(fmt_str, ...)                                           \
    (std::cout << sp << fmt::format(fmt_str __VA_OPT__(, ) __VA_ARGS__) \
               << std::endl)

// End Debug

namespace SysYust::AST::Interpreter {

class Interpreter : public NodeExecutorBase {
   public:
    enum CFDType { CFDBreak = 0, CFDContinue, CFDReturn };
    /// enum ControlFlowData = Break | Continue | Return Option<Value>;
    using ControlFlowData = std::variant<Break, Continue, std::optional<Value>>;
    using Context = SymbolTable<Value>;
    using ReturnType = std::variant<Void, ControlFlowData, Value>;

    void execute(const VarDecl &) override;
    void execute(const FuncDecl &) override;
    void execute(const ParamDecl &) override;

    void execute(const IntLiteral &) override;
    void execute(const FloatLiteral &) override;
    void execute(const Call &) override;
    void execute(const UnaryOp &) override;
    void execute(const BinaryOp &) override;
    void execute(const DeclRef &) override;
    void execute(const ArrayRef &) override;
    void execute(const ToInt &) override;
    void execute(const ToFloat &) override;

    void execute(const Not &) override;
    void execute(const And &) override;
    void execute(const Or &) override;
    void execute(const Compare &) override;
    void execute(const ToCond &) override;

    void execute(const If &) override;
    void execute(const While &) override;
    void execute(const Break &) override;
    void execute(const Continue &) override;
    void execute(const Return &) override;

    void execute(const Assign &) override;
    void execute(const Block &) override;
    void execute(const Empty &) override;

    [[nodiscard]] Value evalExpr(Node &node) {
        // assert node in {IntLiteral, FloatLiteral, Call, UnaryOp, BinaryOp,
        // DeclRef, ArrayRef, ToInt, ToFloat}
        node.execute(this);
        assert(!_isNone());
        assert(!_isCFD());
        auto val = get<Value>(_return_value);
        _return_value = Void_v;
        return val;
    }

    int enter(SyntaxTree *ast);

   private:
    ReturnType _return_value = Void_v;
    std::stack<Context> _context_stack;
    std::stack<Env> _env_stack;
    SyntaxTree *_ast;

    Context &getContext() {
        // Debug


        //  /**
        //   * @brief 打印局部符号表
        //   * 
        //   */
        //  std::string toString() {
        //     std::string ret = typeid(E).name();
        //     for (auto &ent : _local_entry) {
        //         ret += fmt::format("● {} = {}\n", ent.first, ent.second.toString());
        //     }
        //     return ret;
        //  }

        // println("current Context");
        // println("{}", _context_stack.top().toString());
        return _context_stack.top();
    }

    Env &curEnv() {
        return _env_stack.top();
    }

    [[nodiscard]] std::optional<ControlFlowData> _executeCF(const Node &node) {
        // assert node in {If, While, Break, Continue, Return, Block}
        // const_cast<Node&>(node).execute(this);
        assert(!_isNone());
        assert(!_isValue());
        if (_isNone()) {
            return std::nullopt;
        } else {
            auto cfd = get<ControlFlowData>(_return_value);
            _return_value = Void_v;
            return cfd;
        }
    }

    bool _isNone() const {
        return _return_value.index() == 0;
    }

    bool _isCFD() const {
        return _return_value.index() == 1;
    }

    bool _isValue() const {
        return _return_value.index() == 2;
    }

    void passDown(ControlFlowData cfd) {
        _return_value = cfd;
    }

    void _return(ReturnType ret) {
        _return_value = ret;
    }

    template <class T>
    struct Unary {
        static Value calc(UnaryOp::OpType op_type, Value val);
    };
    template <class T>
    struct Binary {
        static Value calc(BinaryOp::OpType, Value, Value);
    };
    template <class T>
    struct Comp {
        static Value calc(Compare::CompareType, Value, Value);
    };
    template <class OuterT, class InnerT>
    struct ArrPtr {
        static Value calc(const ArrayRef &node, Value &val);
    };

    template <class T>
    using ArrayT = ArrPtr<Array, T>;
    template <class T>
    using PointerT = ArrPtr<Pointer, T>;

    template <template <typename> typename F, typename... Ts, typename... Args>
    [[nodiscard]] static Value selector(const Type *type, Args... args) {
        Value ret;
        bool exec = ((getTypeIdOf<Ts> == type->type() &&
                      (ret = F<Ts>::calc(args...), true)) ||
                     ...);
        assert(exec && "Exist Type Not Match");
        assert(ret.isUndef());
        return ret;
    }

    bool bulitinFunc(FuncInfo &func_info, std::vector<Value> &arg_vals);

    Env &curEnv() const;
};

template <class T>
Value Interpreter::Unary<T>::calc(UnaryOp::OpType op_type, Value val) {
    auto operand = val.get<T>();

    Value::val_t<T> ret;
    switch (op_type) {
        case UnaryOp::Positive:
            ret = operand;
            break;
        case UnaryOp::Negative:
            ret = -operand;
            break;
        default:
            assert(false && "Bug");
    }
    auto constexpr type = eq_v<T, Int> ? static_cast<const Type *>(&Int_v)
                                       : static_cast<const Type *>(&Float_v);
    return Value(type, ret);
}

template <class T>
Value Interpreter::Binary<T>::calc(BinaryOp::OpType op_type, Value lv,
                                   Value rv) {
    auto lhs = lv.get<T>(), rhs = rv.get<T>();

    Value::val_t<T> ret;
    switch (op_type) {
        case BinaryOp::Add:
            ret = lhs + rhs;
            break;
        case BinaryOp::Sub:
            ret = lhs - rhs;
            break;
        case BinaryOp::Mul:
            ret = lhs * rhs;
            break;
        case BinaryOp::Div:
            assert(rhs != 0);
            ret = lhs / rhs;
            break;
        case BinaryOp::Mod:
            if constexpr (eq_v<T, Int>) {
                assert(rhs != 0);
                ret = lhs % rhs;
            }
            break;
        default:
            assert(false && "Bug");
    }
    auto constexpr type = eq_v<T, Int> ? static_cast<const Type *>(&Int_v)
                                       : static_cast<const Type *>(&Float_v);
    return Value(type, ret);
}

template <class T>
Value Interpreter::Comp<T>::calc(Compare::CompareType op_type, Value lv,
                                 Value rv) {
    auto lhs = lv.get<T>(), rhs = rv.get<T>();

    Value::Int_t ret = 0;
    switch (op_type) {
        case Compare::EQ:
            ret = lhs == rhs;
            break;
        case Compare::NE:
            ret = lhs != rhs;
            break;
        case Compare::GT:
            ret = lhs > rhs;
            break;
        case Compare::GE:
            ret = lhs >= rhs;
            break;
        case Compare::LT:
            ret = lhs < rhs;
            break;
        case Compare::LE:
            ret = lhs <= rhs;
            break;
        default:
            assert(false && "Bug");
    }
    return Value(&Int_v, ret);
}

template <class OuterT, class InnerT>
Value Interpreter::ArrPtr<OuterT, InnerT>::calc(const ArrayRef &node,
                                                Value &val) {
    auto arr_type = dynamic_cast<const OuterT *>(val.type);
    auto &deref_type = arr_type->index(node.subscripts.size());
    auto offset = arr_type->offsetWith(const_cast<ArrayRef &>(node).subscripts);
    auto deref_val = &get<Value::val_t<InnerT> *>(val._value)[offset];
    return Value(&deref_type, deref_val);
}

}  // namespace SysYust::AST::Interpreter


#endif  // SYSYUST_INTERPRETER_H