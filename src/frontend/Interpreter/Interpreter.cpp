#include "Interpreter/Interpreter.h"

#include <fmt/args.h>
#include <fmt/format.h>

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "AST/Env/FuncInfo.h"
#include "AST/Env/SymbolTable.h"
#include "AST/Node/BinaryOp.h"
#include "AST/Node/Compare.h"
#include "AST/Node/Continue.h"
#include "AST/Node/FuncDecl.h"
#include "AST/Node/NodeBase.h"
#include "AST/Node/UnaryOp.h"
#include "AST/Type/Array.h"
#include "AST/Type/Float.h"
#include "AST/Type/Int.h"
#include "AST/Type/Pointer.h"
#include "AST/Type/TypeBase.h"
#include "Interpreter/Value.h"
#include "Interpreter/sylib.h"
#include "Logger.h"
#include "fmt/core.h"

std::ostream &operator<<(std::ostream &os, sp_t &sp) {
    for (int i = 1; i <= sp.counter; ++i) {
        os << "  ";
    }
    return os;
}

#ifdef USE_PRINTLN

#define enterln(fmt_str, ...) println(fmt_str, __VA_ARGS__), sp.push();
#define exitln(fmt_str, ...)              \
    sp.pop(), println(fmt_str " (-> {})", \
                      __VA_ARGS__ __VA_OPT__(, ) toString(_return_value));
#else

#define enterln(fmt_str, ...)
#define exitln(fmt_str, ...)

#endif

#ifdef USE_SPRINTLN
#define senterln(fmt_str, ...) sprintln(fmt_str, __VA_ARGS__), sp.push();
#define sexitln(fmt_str, ...)              \
    sp.pop(), sprintln(fmt_str " (-> {})", \
                       __VA_ARGS__ __VA_OPT__(, ) toString(_return_value));
#else
#define senterln(fmt_str, ...)
#define sexitln(fmt_str, ...)
#endif

namespace SysYust::AST::Interpreter {

/* --------------- Var --------------- */

Interpreter::ReturnType Interpreter::None = {};
Interpreter::ControlFlowData Interpreter::CFDBreak_v{std::in_place_index<0>,
                                                     std::monostate{}};
Interpreter::ControlFlowData Interpreter::CFDContinue_v{std::in_place_index<1>,
                                                        std::monostate{}};

/* -------------- Enter -------------- */

int Interpreter::enter(SyntaxTree *ast) {
    _ast = ast;
    enterln("Enter TopLevel");
    senterln("Enter TopLevel");

    pushCtxEnv(*_ast->topEnv(), true);

    auto &main_func_decl = *dynamic_cast<FuncDecl *>(_ast->getNode(
        curEnv().func_table.getInfo(curEnv().getId("main")).node));
    auto &main_func = *_ast->getNode(main_func_decl.entry_node);

    printEnv();

    // Load global Var
    auto &env = curEnv();
    auto seq = env.var_table.getSequence();
    for (auto gind : seq) {
        auto &ctx = getContext();
        auto gvar = env.var_table.getInfo(gind);
        auto var_decl = gvar.decl;
        auto &var_node = *_ast->getNode(var_decl);
        var_node.execute(this);
    }

    printCtx();

    int ret = [&] {
        enterln("Enter main");
        senterln("Enter main");

        SyLib::before_main();
        main_func.execute(this);
        SyLib::after_main();

        // Must return int
        auto ret_val =
            std::get<CFDReturn>(std::get<ControlFlowData>(_return_value))
                .value();
        int ret = ret_val.get<Int>();

        exitln("Exit main -> {}", ret);
        sexitln("Exit main -> {}", ret);
        return ret;
    }();

    assert(_context_stack.size() == 1);
    popCtxEnv();

    exitln("Exit TopLevel");
    sexitln("Exit TopLevel");
    return ret;
}

/* -------------- Decl -------------- */

Interpreter::InitList Interpreter::parseInitList(const Type &init_type,
                                                 std::vector<HNode> &inits) {
    if (init_type.isBasicType()) {
        assert(inits.size() <= 1);
        if (inits.size()) {
            auto val_node = inits[0];
            stashPush();
            auto val = evalExpr(*_ast->getNode(val_node)).toRValue();
            auto discard = getExprStr();
            stashPop();
            return {val};
        } else {
            return {{}};
        }
    } else {
        auto arr_type_ptr =
            dynamic_cast<Array *>(&const_cast<Type &>(init_type));
        assert(arr_type_ptr && "Not an array");
        auto arr_type = *arr_type_ptr;

        if (!inits.size()) {
            return {{}};
        }

        auto &deref_type = const_cast<Type &>(arr_type.index(1));
        long long elem_size = 1;
        if (deref_type.type() == TypeId::Array) {
            auto arr = dynamic_cast<const Array &>(deref_type);
            elem_size = arr.size();
        }

        std::vector<InitList> init_list;
        std::vector<HNode> bares;
        for (auto i : inits) {
            auto node_ptr = _ast->getNode(i);
            auto list_ptr = dynamic_cast<List *>(node_ptr);
            if (list_ptr) {
                assert(deref_type.type() == TypeId::Array &&
                       "init list {} depth > array type depth");
                assert(bares.size() == elem_size ||
                       bares.empty() && "Init list is incomplete or excess");
                if (bares.size()) {
                    init_list.push_back(parseInitList(deref_type, bares));
                    bares.clear();
                }
                init_list.push_back(parseInitList(deref_type, list_ptr->vals));
            } else {
                auto val_ptr = dynamic_cast<Node *>(node_ptr);
                assert(val_ptr && "init list val_node is null");
                bares.push_back(i);
            }
        }

        int len = bares.size();
        if (len) {
            int k = len / elem_size, r = len % elem_size;
            for (int i = 1; i <= k; ++i) {
                std::vector<HNode> part(bares.begin() + (i - 1) * elem_size,
                                        bares.begin() + i * elem_size);
                init_list.push_back(parseInitList(deref_type, part));
            }
            if (r) {
                std::vector<HNode> part(bares.begin() + k * elem_size,
                                        bares.begin() + len);
                init_list.push_back(parseInitList(deref_type, part));
            }
        }
        return {init_list};
    }
}

void Interpreter::printInitList(Interpreter::InitList &inits, int ind) {
    switch (inits.list.index()) {
        case 0: {  // Vec<List>
            auto list = std::get<0>(inits.list);
            if (list.empty()) {
                println("{}", "{}");
            } else {
                auto len = list.size();
                println("|- [{}] = {} elems", ind, len);
                sp.push();
                for (int i = 0; i < len; ++i) {
                    printInitList(list[i], i);
                }
                sp.pop();
            }
            break;
        }
        case 1: {  // Value
            auto &val = std::get<Value>(inits.list);
            println("[{}] {}", ind, val.toString());
            break;
        }
        default:
            assert(false && "init list type err");
    }
}

void Interpreter::execute(const VarDecl &node) {
    enterln("! VarDecl pre seek");

    auto &ctx = getContext();
    auto &env = curEnv();

    auto var_id = node.info_id;
    auto var_decl = env.var_table.getInfo(var_id);

    auto init_str =
        node.init_expr.has_value()
            ? fmt::format(" = (init_id: {})", node.init_expr.value())
            : "";
    sp.pop();
    println("VarDecl {} : {}{}", var_decl.name, var_decl.type->toString(),
            init_str);
    sp.push();

    Value debug_val;
    if (node.init_expr.has_value()) {
        auto &init_node = *_ast->getNode(node.init_expr.value());
        auto list_cast = dynamic_cast<List *>(&init_node);
        if (list_cast) {
            auto &list_node = *list_cast;

            assert(var_decl.type->type() == TypeId::Array);
            auto &var_type =
                dynamic_cast<Array &>(const_cast<Type &>(*var_decl.type));
            auto init_list = parseInitList(var_type, list_node.vals);
            printInitList(init_list);
            MemorySlice mslice(var_type);
            if (var_type.baseType().type() == TypeId::Int) {
                FillInitListMemory<Int>(var_type, mslice, init_list);
            } else {
                assert(var_type.baseType().type() == TypeId::Float);
                FillInitListMemory<Float>(var_type, mslice, init_list);
            }
            auto val = Value(&var_type, mslice, true);
            debug_val = val;
            ctx.setInfo(var_id, val);
        } else {
            auto expr_cast = dynamic_cast<Expr *>(&init_node);
            if (expr_cast) {
                stashPush();
                Value val = evalExpr(init_node).toRValue();
                auto discard = getExprStr();
                stashPop();
                assert(val.type->isBasicType());
                assert(val.type == var_decl.type);
                auto lval = val.toLValue();
                ctx.setInfo(var_id, lval);
                debug_val = lval;
            } else {
                assert(false && "Wrong init list");
            }
        }
    } else {
        auto &var_type = *var_decl.type;
        MemorySlice mslice(var_type);
        auto val = Value(&var_type, mslice, true);
        ctx.setInfo(var_id, val);
        debug_val = val;
    }

    exitln("~VarDecl {}", var_decl.name);
    // VarDecl a(id) : type = value
    sprintln("VarDecl {}({}) : {} = {}", var_decl.name, var_id,
             var_decl.type->toString(), debug_val.toString());
}

void Interpreter::execute(const FuncDecl &node) {
    // Not Visit (
    println("FuncDecl!");
}

void Interpreter::execute(const ParamDecl &node) {
    auto &ctx = getContext();
    auto &env = curEnv();

    auto var_id = node.info_id;
    auto var_info = env.var_table.getInfo(var_id);
    auto var_type = var_info.type;

    // Call pass binding value to ParamDecl
    assert(_isValue());
    auto &val = std::get<Value>(_return_value);

    enterln("DeclParmam {}", toString(var_info));

    // validate
    if (val.type->isBasicType()) {
        // OK
    } else if ((val.type->type() == TypeId::Array ||
                val.type->type() == TypeId::Pointer)) {
        assert(var_type->type() == TypeId::Pointer);
    } else {
        assert(false && "ParamDecl value type err");
    }

    assert(*var_type == *val.type);
    auto rval = val.toRValue();  // copy
    auto lval = rval.toLValue();
    ctx.setInfo(var_id, lval);

    // Clear
    _return_value = None;

    exitln("DeclParam {}", var_info.name);
    // DeclParam a : type = value from b
    sprintln("DeclParam {} : {} = {}", var_info.name, var_type->toString(),
             lval.toString());
}

/* -------------- Expr -------------- */

void Interpreter::execute(const IntLiteral &node) {
    _return(Value(&Int_v, node.value));
    println("IntLiteral {}", std::get<Value>(_return_value).toString());
    exprStrFmt("{}", std::get<Value>(_return_value).toString());
}

void Interpreter::execute(const FloatLiteral &node) {
    _return(Value(&Float_v, node.value));
    println("FloatLiteral {}", std::get<Value>(_return_value).toString());
    exprStrFmt("{}", std::get<Value>(_return_value).toString());
}

void Interpreter::execute(const ToInt &node) {
    enterln("ToInt");

    auto &op_node = *_ast->getNode(node.operant);
    auto val = evalExpr(op_node).toRValue();
    assert(val.type->type() == TypeId::Float);

    auto casted = static_cast<Value::Int_t>(val.get<Float>());

    _return(Value(&Int_v, casted));

    exitln("~ToInt {}", val.toString());
    auto op_str = costExprStr();
    exprStrFmt("({} <- ToInt {})", std::get<Value>(_return_value).toString(),
               op_str);
}

void Interpreter::execute(const ToFloat &node) {
    enterln("ToFloat");

    auto &op_node = *_ast->getNode(node.operant);
    auto val = evalExpr(op_node).toRValue();
    assert(val.type->type() == TypeId::Int);

    auto casted = static_cast<Value::Float_t>(val.get<Int>());

    _return(Value(&Float_v, casted));

    exitln("~ToFloat {} -> {}", val.toString(),
           std::get<Value>(_return_value).toString());
    auto op_str = costExprStr();
    exprStrFmt("({} <- ToFloat {})", std::get<Value>(_return_value).toString(),
               op_str);
}

void Interpreter::execute(const UnaryOp &node) {
    enterln("UnaryOp type: {}", toString(node.type));

    Value operand = evalExpr(*_ast->getNode(node.subexpr)).toRValue();
    _return(selector<Unary, Int, Float>(operand.type, node.type, operand));

    exitln("~UnaryOp {} {} -> {}", toString(node.type), operand.toString(),
           std::get<Value>(_return_value).toString());

    auto op_str = costExprStr();
    exprStrFmt("({} <- {} {})", std::get<Value>(_return_value).toString(),
               toString(node.type), op_str);
}

void Interpreter::execute(const BinaryOp &node) {
    enterln("BinaryOp type: {}", toString(node.type));

    auto lhs = evalExpr(*_ast->getNode(node.lhs)).toRValue(),
         rhs = evalExpr(*_ast->getNode(node.rhs)).toRValue();
    assert(lhs.type == rhs.type);
    _return(selector<Binary, Int, Float>(lhs.type, node.type, lhs, rhs));

    exitln("~BinaryOp {} {} {} -> {}", lhs.toString(), toString(node.type),
           rhs.toString(), std::get<Value>(_return_value).toString());

    auto rhs_str = costExprStr();
    auto lhs_str = costExprStr();
    exprStrFmt("({} <- {} {} {})", std::get<Value>(_return_value).toString(),
               lhs_str, toString(node.type), rhs_str);
}

bool Interpreter::bulitinFunc(const FuncInfo &func_info,
                              std::vector<Value> &arg_vals) {
    auto name = func_info.name;
#define DECL_LIB_FUNC(fname, ret_t)                \
    if (name == #fname) {                          \
        Value ret = Value(&ret_t, SyLib::fname()); \
        _return(ret);                              \
        return true;                               \
    } else

    DECL_LIB_FUNC(getint, Int_v)
    DECL_LIB_FUNC(getch, Int_v)
    DECL_LIB_FUNC(getfloat, Float_v)

#undef DECL_LIB_FUNC

#define DECL_LIB_FUNC(fname, arg_t)                     \
    if (name == #fname) {                               \
        assert(arg_vals.size() == 1);                   \
        auto arg = arg_vals[0].toRValue();              \
        auto get_arg = std::get_if<arg_t>(&arg._value); \
        assert(get_arg);                                \
        SyLib::fname(*get_arg);                         \
        return true;                                    \
    } else

    DECL_LIB_FUNC(putint, Value::Int_t)
    DECL_LIB_FUNC(putch, Value::Int_t)
    DECL_LIB_FUNC(starttime, Value::Int_t)
    DECL_LIB_FUNC(stoptime, Value::Int_t)
    DECL_LIB_FUNC(putfloat, Value::Float_t)

#undef DECL_LIB_FUNC

#define DECL_LIB_FUNC(fname, arg_t, ret_t)       \
    if (name == #fname) {                        \
        assert(arg_vals.size() == 1);            \
        auto arg = arg_vals[0];                  \
        auto ptr = arg.getPtr();                 \
        assert(ptr);                             \
        auto ret_val = SyLib::fname((arg_t)ptr); \
        Value ret = Value(&ret_t, ret_val);      \
        _return(ret);                            \
        return true;                             \
    }

    DECL_LIB_FUNC(getarray, Value::Int_t *, Int_v)
    DECL_LIB_FUNC(getfarray, Value::Float_t *, Int_v)

#undef DECL_LIB_FUNC

#define DECL_LIB_FUNC(fname, arg1_t, arg2_t)                    \
    if (name == #fname) {                                       \
        assert(arg_vals.size() == 2);                           \
        auto arg1 = arg_vals[0].toRValue(), arg2 = arg_vals[1]; \
        auto get_arg1 = std::get_if<arg1_t>(&arg1._value);      \
        auto get_arg2 = arg2.getPtr();                          \
        assert(get_arg1 &&get_arg2);                            \
        SyLib::fname(*get_arg1, (arg2_t)get_arg2);              \
        return true;                                            \
    } else

    DECL_LIB_FUNC(putarray, Value::Int_t, Value::Int_t *)
    DECL_LIB_FUNC(putfarray, Value::Int_t, Value::Float_t *)

#undef DECL_LIB_FUNC

    if (name == "putf") {
        assert(arg_vals.size() >= 1);
        // std::string_view fmt_str((char
        // *)std::get<Value::Int_t*>(arg_vals[0]._value)); auto store =
        // fmt::dynamic_format_arg_store<fmt::format_context>(); for (auto arg:
        // arg_vals) store.push_back(arg);

        LOG_WARN("未实现");
        assert(false && "未实现");
        return true;
    }

    return false;
}

void Interpreter::execute(const Call &node) {
    auto &env = curEnv();
    auto &func_info = env.func_table.getInfo(node.func_info);

    SPRINT(auto expr_call = expr_strings.empty() ? "" : "expr ";)
    sprintln("! {}Call {}, calc params:", expr_call, func_info.name);
    stashPush();

    // Arguments evaluation
    std::vector<Value> arg_vals;
    for (auto arg : node.argumentExpr) {
        auto &arg_node = *_ast->getNode(arg);
        stashPush();
        auto arg_val = evalExpr(arg_node).toRValue();
        sprintln("! arg: {}", getExprStr());
        stashPop();
        arg_vals.push_back(arg_val);
    }

    enterln("Call {}", toString(func_info, node.func_info));
    senterln("{}Call {}", expr_call, toString(func_info, node.func_info));

    printEnv();
    printCtx();

    if (bulitinFunc(func_info, arg_vals)) {
        exitln("~Call bulitin {}", func_info.name);
        sexitln("~{}Call bulitin {}", expr_call, func_info.name);
        stashPop();
        // (val <- name(...))
        if (!_isNone())
            exprStrFmt("({} <- {}(...))",
                       std::get<Value>(_return_value).toString(),
                       func_info.name);
        return;
    }

    const auto &ret_type = func_info.type->getResult();
    const auto &func_decl =
        *dynamic_cast<FuncDecl *>(_ast->getNode(func_info.node));

    pushFuncCallEnv(*_ast->seekEnv(&func_decl));

    // Params
    assert(func_decl.param.size() == node.argumentExpr.size());
    assert(_isNone());
    size_t len = func_decl.param.size();
    for (size_t i = 0; i < len; ++i) {
        auto &param_node = *_ast->getNode(func_decl.param[i]);

        // Pass value to ParamDecl
        _return_value = arg_vals[i];
        param_node.execute(this);
        assert(_isNone());
    }

    printCtx();

    auto &func_body = *_ast->getNode(func_decl.entry_node);
    // Run
    auto opt_cfd = _executeCF(func_body);

    if (opt_cfd.has_value()) {
        auto cfd = opt_cfd.value();
        if (ret_type.type() == TypeId::Void) {
            // return;
            assert(cfd.index() == CFDReturn &&
                   !std::get<CFDReturn>(cfd).has_value());
            _return(None);
        } else {
            // return <ret>;
            assert(ret_type.isBasicType());
            auto cfd_ret = get<CFDReturn>(cfd);
            assert(cfd_ret.has_value());
            auto ret = cfd_ret.value();
            assert(ret_type.type() == ret.type->type());
            _return(ret);
        }
    } else {
        // No explict return statement
        assert(ret_type.type() == TypeId::Void);
    }

    popCtxEnv();

    exitln("~Call {}", func_info.name);
    sexitln("~{}Call {}", expr_call, func_info.name);
    stashPop();
    // (val <- name(...))
    if (!_isNone())
        exprStrFmt("({} <- {}(...))", std::get<Value>(_return_value).toString(),
                   func_info.name);
}

void Interpreter::execute(const DeclRef &node) {
    // println("! DeclRef pre seek");

    // printEnv();
    auto &env = curEnv();
    VarInfo var_info = env.var_table.getInfo(node.var_id);

    // getInfoTrace(node.var_id);
    auto &val = getContext().getInfo(node.var_id);
    assert(val.isLValue() && !val.isUndef());

    _return(val);

    println("DeclRef {} -> {} ({})", var_info.name,
            const_cast<Value &>(val).toString(), toString(var_info));
    // (val <- a)
    exprStrFmt("({} <- {})", const_cast<Value &>(val).toString(),
               var_info.name);
}

void Interpreter::execute(const ArrayRef &node) {
    // println("! ArrayRef pre seek");
    auto &env = curEnv();
    auto &var_info = env.var_table.getInfo(node.var_id);
    auto var_type = var_info.type;

    // getInfoTrace(node.var_id);
    auto val = getContext().getInfo(node.var_id);

    println("! ArrayRef indexes");
    // sprintln("! ArrayRef indexes");

    std::vector<std::size_t> val_scripts;
    SPRINT(std::vector<std::string> str_scripts;)
    for (auto i : node.subscripts) {
        auto &val_node = *_ast->getNode(i);
        auto val = evalExpr(val_node).toRValue();
        SPRINT(str_scripts.push_back(costExprStr());)
        assert(val.type->type() == TypeId::Int);
        std::size_t ind = val.get<Int>();
        assert(ind >= 0);
        val_scripts.push_back(ind);
    }
    assert(val_scripts.size() == node.subscripts.size());

    auto val_type = val.type;
    Value ret;
    switch (val_type->type()) {
        case TypeId::Array: {
            auto arr_type = dynamic_cast<const Array *>(val_type);
            ret = selector<ArrayT, Int, Float>(&arr_type->baseType(), node, val,
                                               val_scripts);
            break;
        }
        case TypeId::Pointer: {
            auto ptr_type = dynamic_cast<const Pointer *>(val_type);
            auto &base_type = ptr_type->getBase();
            if (base_type.isBasicType()) {
                ret = selector<PointerT, Int, Float>(&base_type, node, val,
                                                     val_scripts);
            } else if (base_type.type() == TypeId::Array) {
                auto &base =
                    dynamic_cast<const Array *>(&base_type)->baseType();
                assert(base.isBasicType());
                ret = selector<PointerT, Int, Float>(&base, node, val,
                                                     val_scripts);
            }
            break;
        }
        default:
            assert(false && "Only array / pointer can be dereferred");
    }

    _return(ret);

    println("ArrayRef {}[?] -> {} ({})", var_info.name,
            const_cast<Value &>(ret).toString(), toString(var_info));

    // (val <- a[...])
    SPRINT(std::string ind_str; auto len = str_scripts.size();
           for (int i = len - 1; i >= 0; --i) {
               ind_str += fmt::format("{}", str_scripts[i]);
               if (i) ind_str += ", ";
           })
    exprStrFmt("({} <- {}[{}])", ret.toString(), var_info.name, ind_str);
}

/* ------------ CondExpr ------------ */

void Interpreter::execute(const Not &node) {
    enterln("Not");

    auto val = evalExpr(*_ast->getNode(node.subexpr)).toRValue();
    assert(val.type->type() == TypeId::Int);

    auto operand = val.get<Int>();
    _return(Value(&Int_v, operand == 0));

    exitln("~Not !{}", operand);

    auto op_str = costExprStr();
    exprStrFmt("({} <- ! {})", std::get<Value>(_return_value).toString(),
               op_str);
}

void Interpreter::execute(const And &node) {
    enterln("And");

    auto lhs = evalExpr(*_ast->getNode(node.lhs)).toRValue();
    auto lv = lhs.get<Int>();
    assert(0 <= lv && lv <= 1);

    Value::Int_t ret;
    if (lv != 0) {
        auto rhs = evalExpr(*_ast->getNode(node.rhs)).toRValue();
        assert(lhs.type->type() == rhs.type->type());
        auto rv = rhs.get<Int>();
        assert(0 <= rv && rv <= 1);

        ret = rv;
        _return(Value{&Int_v, ret});

        exitln("~And {} && {}", lv, rv);

        auto rhs_str = costExprStr();
        auto lhs_str = costExprStr();
        exprStrFmt("({} <- {} && {})",
                   std::get<Value>(_return_value).toString(), lhs_str, rhs_str);

    } else {
        ret = 0;
        _return(Value{&Int_v, ret});

        exitln("~And {} && -", lv);

        auto lhs_str = costExprStr();
        exprStrFmt("({} <- {} && -)", std::get<Value>(_return_value).toString(),
                   lhs_str);
    }
}

void Interpreter::execute(const Or &node) {
    enterln("Or");
    auto lhs = evalExpr(*_ast->getNode(node.lhs)).toRValue();
    auto lv = lhs.get<Int>();
    assert(0 <= lv && lv <= 1);

    Value::Int_t ret;
    if (lv == 0) {
        auto rhs = evalExpr(*_ast->getNode(node.rhs)).toRValue();
        assert(lhs.type->type() == rhs.type->type());
        auto rv = rhs.get<Int>();
        assert(0 <= rv && rv <= 1);

        ret = rv;
        _return(Value{&Int_v, ret});

        exitln("~Or {} || {}", lv, rv);

        auto rhs_str = costExprStr();
        auto lhs_str = costExprStr();
        exprStrFmt("({} <- {} || {})",
                   std::get<Value>(_return_value).toString(), lhs_str, rhs_str);
    } else {
        ret = 1;
        _return(Value{&Int_v, ret});

        exitln("~Or {} || - ", lv);

        auto lhs_str = costExprStr();
        exprStrFmt("({} <- {} || -)", std::get<Value>(_return_value).toString(),
                   lhs_str);
    }
}

void Interpreter::execute(const Compare &node) {
    enterln("Compare type: {}", toString(node.type));

    auto lhs = evalExpr(*_ast->getNode(node.lhs)).toRValue(),
         rhs = evalExpr(*_ast->getNode(node.rhs)).toRValue();
    assert(lhs.type == rhs.type);

    auto ret = selector<Comp, Int, Float>(lhs.type, node.type, lhs, rhs);
    _return(ret);

    exitln("~Compare {} {} {}", lhs.toString(), toString(node.type),
           rhs.toString());

    auto rhs_str = costExprStr();
    auto lhs_str = costExprStr();
    exprStrFmt("({} <- {} {} {})", std::get<Value>(_return_value).toString(),
               lhs_str, toString(node.type), rhs_str);
}

void Interpreter::execute(const ToCond &node) {
    enterln("ToCond");

    auto operand = evalExpr(*_ast->getNode(node.operant)).toRValue();
    int ret = 0;
    if (operand.type == &Int_v) {
        ret = operand.get<Int>() != 0;
    } else if (operand.type == &Float_v) {
        ret = operand.get<Float>() != 0;
    } else {
        assert(false && "ToCond operand type err");
    }

    _return(Value{&Int_v, ret});

    exitln("~ToCond {}", operand.toString());

    auto op_str = costExprStr();
    exprStrFmt("({} <- ToCond {})", std::get<Value>(_return_value).toString(),
               op_str);
}

/* ---------- Control Flow ---------- */

void Interpreter::execute(const If &node) {
    enterln("If");
    senterln("If");

    auto cond_node = evalExpr(*_ast->getNode(node.cond)).toRValue();
    sprintln("Cond: {}", getExprStr());
    assert(cond_node.type->type() == TypeId::Int);
    int cond = cond_node.get<Int>();
    assert(0 <= cond && cond <= 1);

    if (cond) {
        println("! Iftrue:");
        sprintln("! Iftrue:");

        auto &iftrue = *_ast->getNode(node.stmts);
        iftrue.execute(this);
    } else {
        println("! Iffalse:");
        sprintln("! Iffalse:");

        if (node.else_stmt != -1) {
            auto &iffalse = *_ast->getNode(node.else_stmt);
            iffalse.execute(this);
        } else {
            println("! No else_stmt");
            sprintln("! No else_stmt");
        }
    }

    exitln("~If");
    sexitln("~If");
}

void Interpreter::execute(const While &node) {
    enterln("While");
    senterln("While");

    auto &cond_node = *_ast->getNode(node.cond);
    auto &block = *_ast->getNode(node.stmt);
    int cnt = 0;
    while (true) {
        auto cond = evalExpr(cond_node).get<Int>();
        sprintln("! WCond: {}", getExprStr());
        if (!cond) break;

        ++cnt;
        println("! Block iter {}", cnt);
        sprintln("! Block iter {}", cnt);

        auto cfd_opt = _executeCF(block);

        if (cfd_opt.has_value()) {
            auto cfd = cfd_opt.value();
            if (cfd.index() == CFDType::CFDBreak) {
                _return(None);
                println("! Break");
                break;
            } else if (cfd.index() == CFDType::CFDContinue) {
                _return(None);
                println("! Continue");
                continue;
            } else if (cfd.index() == CFDType::CFDReturn) {
                passDown(cfd);
                println("! Return");
                break;
            }
        }
    }

    exitln("~While {} times", cnt);
    sexitln("~While {} times", cnt);
}

void Interpreter::execute(const Break &node) {
    _return(ControlFlowData(CFDBreak_v));
    println("Break");
    sprintln("Break");
}

void Interpreter::execute(const Continue &node) {
    _return(ControlFlowData(CFDContinue_v));
    println("Continue");
    sprintln("Continue");
}

void Interpreter::execute(const Return &node) {
    enterln("Return");
    if (node.returned) {
        auto ret = evalExpr(*_ast->getNode(node.returned)).toRValue();
        _return(ControlFlowData(ret));
        exitln("~Return");
        sprintln("Return ", getExprStr());
    } else {
        _return(ControlFlowData(std::nullopt));
        exitln("~Return");
        sprintln("Return");
    }
}

void Interpreter::execute(const Block &node) {
    enterln("Block");
    senterln("Block");

    // New Context / Env
    pushCtxEnv(*_ast->seekEnv(&node));

    for (auto stmt : node.stmts) {
        auto &stmt_node = *_ast->getNode(stmt);
        stmt_node.execute(this);

        sp.pop();
        printCtx();
        sp.push();

        if (_isNone())
            continue;
        else if (_isValue()) {
            // discard ExprNode
            _return(None);
            println("Discard Value");
            sprintln("Discard {}", getExprStr());
            continue;
        } else if (_isCFD()) {
            // Passdown
            println("Passdown CFD");
            sprintln("Passdown CFD");
            break;
        } else
            assert(false && "Block _return_value err");
    }

    popCtxEnv();

    exitln("~Block");
    sexitln("~Block");
}

/* ---------------------------------- */

void Interpreter::execute(const Assign &node) {
    enterln("Assign");

    auto lv = evalExpr(*_ast->getNode(node.l_val));
    auto lv_str = getExprStr();
    auto rv = evalExpr(*_ast->getNode(node.r_val));
    auto rv_str = getExprStr();
    assert(lv.isLValue());
    auto rval = rv.toRValue();
    lv.assign(rval);

    exitln("~Assign ? = {}", lv.toString());
    sprintln("{} = {}", lv_str, rv_str);
}

void Interpreter::execute(const Empty &node) {
    // empty
    println("Empty");
    sprintln("Empty");
}

}  // namespace SysYust::AST::Interpreter