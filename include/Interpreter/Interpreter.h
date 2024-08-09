/// SysYust AST 的解释器，用于验证 前端 正确性，另给一点信心（

#ifndef SYSYUST_INTERPRETER_H
#define SYSYUST_INTERPRETER_H

#include <any>
#include <cassert>
#include <climits>
#include <iostream>
#include <limits>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "AST/Env/FuncInfo.h"
#include "AST/Env/SymbolTable.h"
#include "AST/Env/VarInfo.h"
#include "AST/Node/ArrayRef.h"
#include "AST/Node/BinaryOp.h"
#include "AST/Node/Compare.h"
#include "AST/Node/Continue.h"
#include "AST/Node/FuncDecl.h"
#include "AST/Node/IntLiteral.h"
#include "AST/Node/ParamDecl.h"
#include "AST/Node/UnaryOp.h"
#include "AST/Node/VarDecl.h"
#include "AST/SyntaxTree.h"
#include "AST/Type/TypeBase.h"
#include "Interpreter/Value.h"
#include "fmt/core.h"

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
    void clear() {
        counter = 0;
    }
    operator std::string () {
        std::string ret;
        for (int i = 1; i <= counter; ++i) ret += "    ";
        return ret;
    }
};

static sp_t sp;

std::ostream &operator<<(std::ostream &os, sp_t &sp);

// #define USE_PRINTLN
#ifdef USE_PRINTLN
#if 1 // print to cout
#define println(fmt_str, ...)                                           \
    (std::cout << sp << fmt::format(fmt_str __VA_OPT__(, ) __VA_ARGS__) \
               << std::endl)
#else // print to log_string
#define println(fmt_str, ...)                                          \
    (log_data << sp << fmt::format(fmt_str __VA_OPT__(, ) __VA_ARGS__) \
              << std::endl)
#endif
#else 
#define println(fmt_str, ...)
#endif

// #define USE_SPRINTLN
#ifdef USE_SPRINTLN
#define SPRINT(x) x
#define sprintln(fmt_str, ...)                                          \
    (std::cout << sp << fmt::format(fmt_str __VA_OPT__(, ) __VA_ARGS__) \
               << std::endl)
#define exprStrFmt(fmt_str, ...) expr_strings.push_back(fmt::format(fmt_str __VA_OPT__(,) __VA_ARGS__))
#else 
#define SPRINT(x)
#define sprintln(fmt_str, ...)
#define exprStrFmt(fmt_str, ...)
#endif


// End Debug


namespace SysYust::AST::Interpreter {

class Interpreter : public NodeExecutorBase {
   public:
    std::stringstream log_data;
    SPRINT(
    std::vector<std::string> expr_strings;
    std::stack<std::vector<std::string>> expr_strings_stash;
    std::string costExprStr() {
        assert(expr_strings.size());
        auto str = expr_strings.back();
        expr_strings.pop_back();
        return str;
    }
    std::string getExprStr() {
        if (expr_strings.size() != 1) {
            sprintln("Err expr: size({})", expr_strings.size());
            for (auto s : expr_strings) {
                sprintln("{}", s);
            }
        }
        assert(expr_strings.size() == 1);
        auto expr_str = expr_strings[0];
        expr_strings.clear();
        return expr_str;
    }
    void stashPush() {
        // std::vector<std::string> push_strings;
        // std::swap(push_strings, expr_strings);
        // expr_strings_stash.push(std::move(push_strings))
        
        expr_strings_stash.push(expr_strings);
        expr_strings.clear();
    }
    void stashPop() {
        assert(!expr_strings_stash.empty());
        assert(expr_strings.size() == 0);
        // expr_strings = std::move(expr_strings_stash.top());
        // expr_strings_stash.pop();
        expr_strings = expr_strings_stash.top();
        expr_strings_stash.pop();
    }
    )
    #ifndef USE_SPRINTLN
    void stashPush() {}
    void stashPop() {}
    int costExprStr() { return 1; }
    int getExprStr() { return 1; }
    #endif
    enum CFDType { CFDBreak = 0, CFDContinue, CFDReturn };
    /// enum ControlFlowData = Break | Continue | Return Option<Value>;
    using ControlFlowData =
        std::variant<std::monostate, std::monostate, std::optional<Value>>;
    static ControlFlowData CFDBreak_v;
    static ControlFlowData CFDContinue_v;

    using Context = SymbolTable<Value>;
    using ReturnType = std::variant<std::monostate, ControlFlowData, Value>;
    static ReturnType None;

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
        _return_value = None;

        return val;
    }

    int enter(SyntaxTree *ast);

    struct InitList {
        std::variant<std::vector<InitList>, Value> list;
    };

   private:
    ReturnType _return_value = None;
    std::stack<Context> _context_stack;
    Context *_global_ctx;
    std::stack<Env> _env_stack;
    SyntaxTree *_ast;

    Context &getContext() {
        assert(!_context_stack.empty());
        return _context_stack.top();
    }

    Env &curEnv() {
        assert(!_env_stack.empty());
        return _env_stack.top();
    }

    void pushCtxEnv(Env &env, bool top_level = 0) {
        _env_stack.push(env);
        assert(top_level || !_context_stack.empty());
        _context_stack.push(top_level ? Context() : Context(&getContext()));
        if (top_level) _global_ctx = &_context_stack.top();
    }

    void pushFuncCallEnv(Env &env) {
        _env_stack.push(env);
        assert(!_context_stack.empty());
        _context_stack.push(*_global_ctx);
    }

    void popCtxEnv() {
        _env_stack.pop();
        _context_stack.pop();
    }

    void printCtx() {
        println("! Ctx layer {}", _context_stack.size());
        bool no_entry = 1;
        for (auto gvar : getContext()) {
            no_entry = 0;
            auto id = gvar.first;
            auto name = curEnv().var_table.getInfo(id).name;
            auto val = gvar.second;
            println("● {} \t= {} \t(id: {})", name, val.toString(), id);
        }
        if (no_entry) println("x No entry.");
    }

    std::string toString(const FuncInfo &func_info, HNode lib_id) {
        auto decl = func_info.node;
        std::string decl_str, id_str;
        if (decl == std::numeric_limits<HNode>::max()) {
            decl_str = "lib_func";
            id_str = lib_id == std::numeric_limits<HNode>::max()
                         ? fmt::format("id: {}, ", lib_id)
                         : "";
        } else {
            auto &decl_node = *dynamic_cast<FuncDecl *>(_ast->getNode(decl));
            id_str = fmt::format("id: {}, ", decl_node.info_id);
            decl_str = fmt::format("{}", decl);
        }

        auto name = func_info.name;
        auto type = func_info.type;
        auto type_str = type ? type->toString() : "(putf)";
        return fmt::format("{} \t: {} \t({}decl_id: {})", name, type_str,
                           id_str, decl_str);
    }

    std::string toString(const VarInfo &var_info) {
        auto decl = var_info.decl;
        int id;
        auto decl_node = dynamic_cast<VarDecl *>(_ast->getNode(decl));
        if (!decl_node) {
            auto decl_node = dynamic_cast<ParamDecl *>(_ast->getNode(decl));
            assert(decl_node);
            id = decl_node->info_id;
        } else {
            id = decl_node->info_id;
        }
        auto id_str = fmt::format("id: {}, ", id);
        auto name = var_info.name;
        auto type = var_info.type;
        auto isConstant = var_info.isConstant;
        auto isParam = var_info.isParam;
        return fmt::format("{} \t: {} \t({}decl_id: {}, const? {}, param? {})",
                           name, type->toString(), id_str, decl, isConstant,
                           isParam);
    }

    void printEnv() {
        println("! Env layer {}", _env_stack.size());

        println("! - func_table");
        bool no_func_entry = 1;
        for (auto func : curEnv().func_table) {
            no_func_entry = 0;
            auto id = func.first;
            auto func_info = func.second;
            println("● {}", toString(func_info, id));
        }
        if (no_func_entry) println("x No entry.");

        println("! - var_table");
        bool no_var_entry = 1;
        for (auto var : curEnv().var_table) {
            no_var_entry = 0;
            auto id = var.first;
            auto var_info = var.second;
            println("● {}", toString(var_info));
        }
        if (no_var_entry) println("x No entry.");
    }

    std::string toString(SysYust::AST::UnaryOp::OpType op) {
        switch (op) {
            case SysYust::AST::UnaryOp::OpType::Positive:
                return "(+)";
            case SysYust::AST::UnaryOp::OpType::Negative:
                return "(-)";
            default:
                assert(0 && "Err Unary OpType");
        }
    }

    std::string toString(SysYust::AST::BinaryOp::OpType op) {
        switch (op) {
            case BinaryOp::OpType::Add:
                return "+";
            case BinaryOp::OpType::Sub:
                return "-";
            case BinaryOp::OpType::Mul:
                return "*";
            case BinaryOp::OpType::Div:
                return "/";
            case BinaryOp::OpType::Mod:
                return "%";
            default:
                assert(0 && "Err BinOp");
        }
    }

    std::string toString(SysYust::AST::Compare::CompareType op) {
        std::string tab[] = {"==", "!=", ">", ">=", "<", "<="};
        return tab[op];
    }

    std::string toString(
        SysYust::AST::Interpreter::Interpreter::ReturnType ret) {
        switch (ret.index()) {
            case 0:  // Void
                return "Void";
            case 1: {
                // CFD
                auto cfd = std::get<
                    SysYust::AST::Interpreter::Interpreter::ControlFlowData>(
                    ret);
                switch (cfd.index()) {
                    case Interpreter::CFDType::CFDBreak:
                        return "Break";
                    case Interpreter::CFDType::CFDContinue:
                        return "Continue";
                    case Interpreter::CFDType::CFDReturn: {
                        auto ret_val = std::get<2>(cfd);
                        if (ret_val.has_value()) {
                            return fmt::format("Return {}",
                                               ret_val.value().toString());
                        } else {
                            return "Return";
                        }
                    }
                    default:
                        assert(0 && "Err Ret");
                }
            }
            case 2:  // Value
                return std::get<2>(ret).toString();
            default:
                assert(0 && "Wrong index");
        }
    }

    void getInfoTrace(NumId id) {
        _getInfoTrace(id, _context_stack.size(), getContext());
    }

    void _getInfoTrace(NumId id, int layer, Context &ctx) {
        LOG_TRACE("[InfoTrace] Seek Env/Ctx symbol info with id {}", id);
        auto var_info = curEnv().var_table.getInfo(id);
        println("In layer {}, find [{}]", layer, toString(var_info));
        for (auto i : ctx) {
            if (i.first == id) {
                // Hit
                auto val = i.second;
                println("! Hit {} -> {}", id,
                        val.isUndef() ? "undef" : val.toString());
                return;
            }
        }
        auto parent = ctx.getParent();
        if (parent) {
            _getInfoTrace(id, layer - 1, *parent);
        } else {
            assert(layer == 1);
            println("! Not Found!");
        }
    }

    [[nodiscard]] std::optional<ControlFlowData> _executeCF(const Node &node) {
        // assert node in {If, While, Break, Continue, Return, Block}
        const_cast<Node &>(node).execute(this);
        if (_isValue()) {
            // discard value
            auto val_str = getExprStr();
            sprintln("Discard: {}", val_str);
            _return_value = None;
            return std::nullopt;
        } else if (_isNone()) {
            return std::nullopt;
        } else {
            auto cfd = get<ControlFlowData>(_return_value);
            _return_value = None;
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
        static Value calc(const ArrayRef &node, Value &val,
                          std::vector<std::size_t> &val_scripts);
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
        assert(!ret.isUndef());
        return ret;
    }

    bool bulitinFunc(const FuncInfo &func_info, std::vector<Value> &arg_vals);

    InitList parseInitList(const Type &init_type, std::vector<HNode> &inits);
    void printInitList(Interpreter::InitList &inits, int ind = 0);

    template <typename T>
    void FillInitListMemory(Type &type, MemorySlice mslice,
                            const Interpreter::InitList &inits);
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
Value Interpreter::ArrPtr<OuterT, InnerT>::calc(
    const ArrayRef &node, Value &val, std::vector<std::size_t> &val_scripts) {
    auto arr_type = dynamic_cast<const OuterT *>(val.type);
    auto &deref_type = arr_type->index(val_scripts.size());

    // assert(deref_type.isBasicType());

    auto offset = arr_type->offsetWith(val_scripts);

    auto mslice = std::get<MemorySlice>(val._value);
    auto ptr = mslice.ptr;  // copy
    auto &pos_void = (static_cast<Value::val_t<InnerT> *>(mslice.pos))[offset];
    auto pos = static_cast<void *>(&pos_void);
    MemorySlice sub_mslice(ptr, pos, 1);
    return Value(&deref_type, sub_mslice, true);
}

template <typename T>
void Interpreter::FillInitListMemory(Type &type, MemorySlice mslice,
                                     const Interpreter::InitList &inits) {
    if (inits.list.index() == 1) {  // Value
        auto val = std::get<Value>(inits.list);
        *mslice.access<T>() = val.get<T>();
    } else {
        auto &arr_type = static_cast<Array &>(type);
        auto &elem_type = const_cast<Type &>(arr_type.index(1));
        long long size = 1;
        if (elem_type.type() == TypeId::Array) {
            auto arr = dynamic_cast<const Array &>(elem_type);
            size = arr.size();
        }
        auto list = std::get<std::vector<InitList>>(inits.list);
        auto pos = mslice.access<T>();
        int offset = 0;
        for (auto i : list) {
            FillInitListMemory<T>(
                elem_type,
                mslice.subSlice(static_cast<void *>(&pos[offset]), offset), i);
            offset += size;
        }
    }
}

}  // namespace SysYust::AST::Interpreter

#endif  // SYSYUST_INTERPRETER_H