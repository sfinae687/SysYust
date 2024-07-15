#include "Interpreter/Interpreter.h"

#include <fmt/args.h>
#include <fmt/format.h>

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <ostream>
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
#include "AST/Type/Void.h"
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

std::string toString(SysYust::AST::Interpreter::Interpreter::ReturnType ret) {
    using enum SysYust::AST::Interpreter::Interpreter::CFDType;
    switch (ret.index()) {
        case 0:  // Void
            return "Void";
        case 1: {
            // CFD
            auto cfd = std::get<
                SysYust::AST::Interpreter::Interpreter::ControlFlowData>(ret);
            switch (cfd.index()) {
                case CFDBreak:
                    return "Break";
                case CFDContinue:
                    return "Continue";
                case CFDReturn: {
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

#define enterln(fmt_str, ...) println(fmt_str, __VA_ARGS__), sp.push();
#define exitln(fmt_str, ...)              \
    sp.pop(), println(fmt_str " (-> {})", \
                      __VA_ARGS__ __VA_OPT__(, ) toString(_return_value));

std::string toString(SysYust::AST::UnaryOp::OpType op) {
    using enum SysYust::AST::UnaryOp::OpType;
    switch (op) {
        case Positive:
            return "(+)";
        case Negative:
            return "(-)";
        default:
            assert(0 && "Err Unary OpType");
    }
}

std::string toString(SysYust::AST::BinaryOp::OpType op) {
    using enum SysYust::AST::BinaryOp::OpType;
    switch (op) {
        case Add:
            return "+";
        case Sub:
            return "-";
        case Mul:
            return "*";
        case Div:
            return "/";
        case Mod:
            return "%";
        default:
            assert(0 && "Err BinOp");
    }
}

std::string toString(SysYust::AST::Compare::CompareType op) {
    using enum SysYust::AST::Compare::CompareType;
    std::string tab[] = {"==", "!=", ">", ">=", "<", "<="};
    return tab[op];
}



namespace SysYust::AST::Interpreter {

/* -------------- Enter -------------- */

int Interpreter::enter(SyntaxTree *ast) {
    enterln("Enter main");
    _ast = ast;
    auto &top_env = *_ast->topEnv();
    _env_stack.push(top_env);
    auto &main_func_decl = *dynamic_cast<FuncDecl *>(
        _ast->getNode(top_env.func_table.getInfo(top_env.getId("main")).node));
    auto &main_func = *_ast->getNode(main_func_decl.entry_node);

    assert(_context_stack.empty());
    _context_stack.push(Context());

    // Load global Var

    SyLib::before_main();
    main_func.execute(this);
    auto ret_val =
        std::get<CFDReturn>(std::get<ControlFlowData>(_return_value)).value();
    int ret = ret_val.get<Int>();
    SyLib::after_main();

    assert(_context_stack.size() == 1);
    _context_stack.pop();
    _env_stack.pop();
    exitln("Exit main -> {}", ret);
    return ret;
}

/* -------------- Decl -------------- */

void Interpreter::execute(const VarDecl &node) {
    enterln("! VarDecl pre seek");

    assert(!_context_stack.empty());
    auto &top_ctx = _context_stack.top();
    auto &env = curEnv();

    // print symtab

    auto var_id = node.info_id;
    auto var_decl = env.var_table.getInfo(var_id);

    enterln("VarDecl [{}] {}", (void *)&node, var_decl.name);

    auto &ctx = getContext();
    if (node.init_expr.has_value()) {
        println("Init = {}", node.init_expr.value());

        auto &init_node = *_ast->getNode(node.init_expr.value());
        auto list_cast = dynamic_cast<List *>(&init_node);
        if (list_cast) {
            LOG_WARN("未实现");
            assert(false && "未实现");
        } else {
            auto expr_cast = dynamic_cast<Expr *>(&init_node);
            if (expr_cast) {
                Value val = evalExpr(init_node);
                assert(val.type->isBasicType());
                assert(val.type == var_decl.type);
                ctx.setInfo(var_id, val.toRValue());
            } else {
                assert(false && "Wrong init list");
            }
        }
    } else {
        ctx.setInfo(var_id, Value());
    }

    exitln("~VarDecl {}", var_decl.name);
}

void Interpreter::execute(const FuncDecl &node) {
    // Not Visit (
    println("FuncDecl!");
}

void Interpreter::execute(const ParamDecl &node) {
    assert(!_context_stack.empty());
    auto &top_ctx = _context_stack.top();
    auto &env = *_ast->seekEnv(const_cast<ParamDecl *>(&node));
    auto var_id = node.info_id;
    auto var_decl = env.var_table.getInfo(var_id);
    auto var_type = var_decl.type;

    auto &ctx = getContext();
    assert(_isValue());
    auto &val = std::get<Value>(_return_value);

    enterln("DeclParam {} : {} = {}", var_decl.name, var_decl.type->toString(),
            val.toString());

    if (val.type->isBasicType()) {
        ctx.setInfo(var_id, val.toRValue());
    } else if (val.is_lvalue && (val.type->type() == TypeId::Array ||
                                 val.type->type() == TypeId::Pointer)) {
        assert(var_type->type() == TypeId::Pointer);
        assert(*var_type == *val.type);
        ctx.setInfo(var_id, val.toRValue());
    } else {
        assert(false && "Param decl err");
    }
    _return_value = Void_v;

    exitln("DeclParam {}", var_decl.name);
}

/* -------------- Expr -------------- */

void Interpreter::execute(const IntLiteral &node) {
    _return(Value(&Int_v, node.value));
    println("IntLiteral {}", std::get<Value>(_return_value).toString());
}

void Interpreter::execute(const FloatLiteral &node) {
    _return(Value(&Float_v, node.value));
    println("FloatLiteral {}", std::get<Value>(_return_value).toString());
}

void Interpreter::execute(const ToInt &node) {
    enterln("ToInt");
    auto &op_node = *_ast->getNode(node.operant);
    auto val = evalExpr(op_node);
    assert(val.type->type() == TypeId::Float);
    _return(Value(&Int_v, static_cast<Value::Int_t>(val.get<Float>())));
    exitln("~ToInt {} -> {}", val.toString(),
           std::get<Value>(_return_value).toString());
}

void Interpreter::execute(const ToFloat &node) {
    enterln("ToFloat");
    auto &op_node = *_ast->getNode(node.operant);
    auto val = evalExpr(op_node);
    assert(val.type->type() == TypeId::Int);
    _return(Value(&Float_v, static_cast<Value::Float_t>(val.get<Int>())));
    exitln("~ToFloat {} -> {}", val.toString(),
           std::get<Value>(_return_value).toString());
}

void Interpreter::execute(const UnaryOp &node) {
    enterln("UnaryOp type: {}", toString(node.type));
    Value operand = evalExpr(*_ast->getNode(node.subexpr));
    _return(selector<Unary, Int, Float>(operand.type, node.type, operand));
    exitln("~UnaryOp {} {} -> {}", toString(node.type), operand.toString(),
           std::get<Value>(_return_value).toString());
}

void Interpreter::execute(const BinaryOp &node) {
    enterln("BinaryOp type: {}", toString(node.type));
    auto lhs = evalExpr(*_ast->getNode(node.lhs)),
         rhs = evalExpr(*_ast->getNode(node.rhs));
    assert(lhs.type == rhs.type);
    _return(selector<Binary, Int, Float>(lhs.type, node.type, lhs, rhs));
    exitln("~BinaryOp {} {} {} -> {}", lhs.toString(), toString(node.type),
           rhs.toString(), std::get<Value>(_return_value).toString());
}

bool Interpreter::bulitinFunc(FuncInfo &func_info,
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

    DECL_LIB_FUNC(getarray, Value::Int_t *)
    DECL_LIB_FUNC(getfarray, Value::Float_t *)

#undef DECL_LIB_FUNC

#define DECL_LIB_FUNC(fname, arg1_t, arg2_t)                               \
    if (name == #fname) {                                                  \
        assert(arg_vals.size() == 2);                                      \
        auto arg1 = arg_vals[0].toRValue(), arg2 = arg_vals[1].toRValue(); \
        auto get_arg1 = std::get_if<arg1_t>(&arg1._value);                 \
        auto get_arg2 = std::get_if<arg2_t>(&arg2._value);                 \
        assert(get_arg1 &&get_arg2);                                       \
        SyLib::fname(*get_arg1, *get_arg2);                                \
        return true;                                                       \
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
    auto sym_tab = _ast->seekEnv(&const_cast<Call &>(node));
    FuncInfo func_info = sym_tab->func_table.getInfo(node.func_info);

    std::vector<Value> arg_vals;
    for (auto arg : node.argumentExpr) {
        auto &arg_node = *_ast->getNode(arg);
        auto arg_val = evalExpr(arg_node);
        arg_vals.push_back(arg_val);
    }

    enterln("Call {} : {}", func_info.name, func_info.type->toString());

    if (bulitinFunc(func_info, arg_vals)) {
        exitln("~Call bulitin {}", func_info.name);
        return;
    }

    _context_stack.push(Context(_context_stack.top()));

    const auto &ret_type = func_info.type->getResult();
    const auto &func_decl =
        *dynamic_cast<FuncDecl *>(_ast->getNode(func_info.node));

    // Params
    assert(func_decl.param.size() == node.argumentExpr.size());
    assert(_isNone());
    size_t len = func_decl.param.size();
    for (size_t i = 0; i < len; ++i) {
        auto &param_node = *_ast->getNode(func_decl.param[i]);

        _return_value = arg_vals[i];
        param_node.execute(this);
        assert(_isNone());
    }

    // Run
    auto opt_cfd = _executeCF(func_decl);

    if (func_info.type->getResult() == Void_v) {
        if (opt_cfd.has_value()) {
            auto cfd = opt_cfd.value();
            assert(cfd.index() == CFDType::CFDReturn);
            auto cfd_ret = get<CFDType::CFDReturn>(cfd);
            assert(!cfd_ret.has_value());
        } else {
            // no return stmt
        }
        _return(Void_v);
    } else {
        assert(func_info.type->getResult().isBasicType());
        assert(opt_cfd.has_value());
        auto cfd = opt_cfd.value();
        assert(cfd.index() == CFDType::CFDReturn);
        auto cfd_ret = get<CFDType::CFDReturn>(cfd);
        assert(cfd_ret.has_value());
        auto retval = cfd_ret.value();
        assert(retval.type == &ret_type);
        _return(retval);
    }

    _context_stack.pop();

    exitln("~Call {}", func_info.name);
}

void Interpreter::execute(const DeclRef &node) {
    auto &sym_tab = curEnv();
    VarInfo var_info = sym_tab.var_table.getInfo(node.var_id);
    const auto &var_type = *var_info.type;
    auto val = getContext().getInfo(node.var_id);
    _return(val.toLValue());
    println("DeclRef {}", var_info.name);
}

void Interpreter::execute(const ArrayRef &node) {
    auto &sym_tab = curEnv();
    VarInfo var_info = sym_tab.var_table.getInfo(node.var_id);
    const auto &var_type = *var_info.type;
    auto val = getContext().getInfo(node.var_id);

    auto val_type = val.type;
    Value ret;
    switch (val_type->type()) {
        case TypeId::Array: {
            auto arr_type = dynamic_cast<const Array *>(val_type);
            ret =
                selector<ArrayT, Int, Float>(&arr_type->baseType(), node, val);
            break;
        }
        case TypeId::Pointer: {
            auto ptr_type = dynamic_cast<const Pointer *>(val_type);
            auto &base_type = ptr_type->getBase();
            if (base_type.isBasicType()) {
                ret = selector<PointerT, Int, Float>(&base_type, node, val);
            } else if (base_type.type() == TypeId::Array) {
                auto &base =
                    dynamic_cast<const Array *>(&base_type)->baseType();
                assert(base.isBasicType());
                ret = selector<PointerT, Int, Float>(&base, node, val);
            }
            break;
        }
        default:
            assert(false && "Only can deref array / pointer");
    }

    _return(ret);
    println("ArrayRef {}", var_info.name);
}

/* ------------ CondExpr ------------ */

void Interpreter::execute(const Not &node) {
    enterln("Not");
    auto val = evalExpr(*_ast->getNode(node.subexpr));
    auto operand = val.get<Int>();
    _return(Value(&Int_v, operand == 0));
    exitln("~Not !{}", operand, operand == 0);
}

void Interpreter::execute(const And &node) {
    enterln("And");
    auto lhs = evalExpr(*_ast->getNode(node.lhs));
    // assert(lhs.type == rhs.type && lhs.type == Int);
    auto lv = lhs.get<Int>();

    Value::Int_t ret;

    assert(0 <= lv && lv <= 1);
    if (lv != 0) {
        auto rhs = evalExpr(*_ast->getNode(node.rhs));
        auto rv = rhs.get<Int>();
        assert(0 <= rv && rv <= 1);

        ret = rv;
        _return(Value{&Int_v, ret});
        exitln("~And {} && {}", lv, rv);
    } else {
        ret = 0;
        _return(Value{&Int_v, ret});
        exitln("~And {} && -", lv);
    }
}

void Interpreter::execute(const Or &node) {
    enterln("Or");
    auto lhs = evalExpr(*_ast->getNode(node.lhs));
    // assert(lhs.type == rhs.type && lhs.type == Int);
    auto lv = lhs.get<Int>();

    Value::Int_t ret;

    assert(0 <= lv && lv <= 1);
    if (lv == 0) {
        auto rhs = evalExpr(*_ast->getNode(node.rhs));
        auto rv = rhs.get<Int>();
        assert(0 <= rv && rv <= 1);

        ret = rv;
        _return(Value{&Int_v, ret});
        exitln("~Or {} || {}", lv, rv);
    } else {
        ret = 0;
        _return(Value{&Int_v, ret});
        exitln("~Or {} || - ", lv);
    }
}

void Interpreter::execute(const Compare &node) {
    enterln("Compare type: {}", toString(node.type));
    auto lhs = evalExpr(*_ast->getNode(node.lhs)),
         rhs = evalExpr(*_ast->getNode(node.rhs));
    assert(lhs.type == rhs.type);
    auto ret = selector<Comp, Int, Float>(lhs.type, node.type, lhs, rhs);
    _return(ret);
    exitln("~Compare {} {} {}", lhs.get<Int>(), toString(node.type),
           rhs.get<Int>());
}

void Interpreter::execute(const ToCond &node) {
    enterln("ToCond");
    auto operand = evalExpr(*_ast->getNode(node.operant));
    int ret = 0;
    if (operand.type == &Int_v) {
        ret = operand.get<Int>() != 0;
    } else if (operand.type == &Float_v) {
        ret = operand.get<Float>() != 0;
    } else {
        assert(true && "bug");
    }
    _return(Value{operand.type, ret});
    exitln("~ToCond {}", operand.toString());
}

/* ---------- Control Flow ---------- */

void Interpreter::execute(const If &node) {
    enterln("If") auto cond = evalExpr(*_ast->getNode(node.cond));
    assert(cond.type == &Int_v);
    int val = cond.get<Int>();
    assert(0 <= val && val <= 1);
    if (val) {
        println("Iftrue:");
        auto &iftrue = *_ast->getNode(node.stmts);
        iftrue.execute(this);
    } else {
        println("Iffalse:");
        if (node.else_stmt != -1) {
            auto &iffalse = *_ast->getNode(node.else_stmt);
            iffalse.execute(this);
        }
    }
    exitln("~If");
}

void Interpreter::execute(const While &node) {
    enterln("While");
    int cnt = 0;
    auto &condnode = *_ast->getNode(node.cond);
    // assert(dynamic_cast<CondExpr &>(condnode) != nullptr);
    auto &block = *_ast->getNode(node.stmt);
    while (true) {
        auto cond = evalExpr(condnode).get<Int>();
        if (!cond) break;

        auto cfd_opt = _executeCF(block);

        cnt++;

        if (cfd_opt.has_value()) {
            auto cfd = cfd_opt.value();
            if (cfd.index() == CFDType::CFDBreak) {
                _return(Void_v);
                println("Break!");
                break;
            } else if (cfd.index() == CFDType::CFDContinue) {
                _return(Void_v);
                println("Continue!");
                continue;
            } else if (cfd.index() == CFDType::CFDReturn) {
                passDown(cfd);
                println("Return!");
                break;
            }
        }
    }
    exitln("~While {} times", cnt);
}

void Interpreter::execute(const Break &node) {
    _return(ControlFlowData(node));
    println("Break");
}

void Interpreter::execute(const Continue &node) {
    _return(ControlFlowData(node));
    println("Continue");
}

void Interpreter::execute(const Return &node) {
    enterln("Return");
    if (node.returned) {
        auto retval = evalExpr(*_ast->getNode(node.returned));
        _return(ControlFlowData(retval));
        exitln("~Return");
    } else {
        _return(ControlFlowData(std::nullopt));
        exitln("~Return");
    }
}

void Interpreter::execute(const Block &node) {
    enterln("Block");
    // New Context
    assert(!_context_stack.empty());
    _context_stack.push(Context(_context_stack.top()));
    assert(!_env_stack.empty());
    auto &cur_env = *_ast->seekEnv(&node);
    _env_stack.push(cur_env);

    for (auto stmt : node.stmts) {
        auto &stmt_node = *_ast->getNode(stmt);
        stmt_node.execute(this);
        if (_isNone())
            continue;
        else if (_isValue()) {
            _return_value = Void_v;  // discard ExprNode
            println("Discard Value");
            continue;
        } else if (_isCFD()) {
            // Passdown
            println("Passdown CFD");
            break;
        } else
            assert(0 && "bug");
    }

    _context_stack.pop();
    _env_stack.pop();
    exitln("~Block");
}

/* ---------------------------------- */

void Interpreter::execute(const Assign &node) {
    enterln("Assign");
    auto lv = evalExpr(*_ast->getNode(node.l_val));
    auto rv = evalExpr(*_ast->getNode(node.r_val));
    lv.assign(rv);
    exitln("~Assign ? = {}", rv.toString());
}

void Interpreter::execute(const Empty &node) {
    // empty
    println("Empty");
}

}  // namespace SysYust::AST::Interpreter