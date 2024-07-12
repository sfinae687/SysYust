#include "Interpreter/Interpreter.h"

#include <cassert>
#include <cstdint>
#include <optional>
#include <variant>

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

namespace SysYust::AST::Interpreter {

/* -------------- Enter -------------- */

int Interpreter::enter(SyntaxTree *ast) {
    _ast = ast;
    auto &top_env = *_ast->topEnv();
    auto &main_func_decl = *_ast->getNode(top_env.getId("main"));
    auto &main_func = *_ast->getNode(dynamic_cast<FuncDecl *>(&main_func_decl)->entry_node);
    SyLib::before_main();
    main_func.execute(this);
    auto ret_val = std::get<CFDReturn>(std::get<ControlFlowData>(_return_value)).value();
    int ret = ret_val.get<Int>();
    SyLib::after_main();
    return ret;
}

/* -------------- Decl -------------- */

void Interpreter::execute(const VarDecl &node) {
    // auto &top_ctx = _context_stack.top();
    // auto var_id = node.info_id;

    LOG_WARN("未实现");
    assert(0 && "未实现");
}

void Interpreter::execute(const FuncDecl &node) {
    // TODO: FuncParam
    
}

void Interpreter::execute(const ParamDecl &node) {
    LOG_WARN("未实现");
    assert(0 && "未实现");
}

/* -------------- Expr -------------- */

void Interpreter::execute(const IntLiteral &node) {
    _return(Value(&Int_v, node.value));
    // LOG_TRACE("IntLiteral -> {}", get<Value>(_return_value).toString());
}

void Interpreter::execute(const FloatLiteral &node) {
    _return(Value(&Float_v, node.value));
}

void Interpreter::execute(const ToInt &node) {
    _return(Value(&Int_v, static_cast<std::int32_t>(node.operant)));
}

void Interpreter::execute(const ToFloat &node) {
    _return(Value(&Float_v, static_cast<float>(node.operant)));
}

void Interpreter::execute(const UnaryOp &node) {
    Value operand = evalExpr(*_ast->getNode(node.subexpr));
    _return(selector<Unary, Int, Float>(operand.type, node.type, operand));
}

void Interpreter::execute(const BinaryOp &node) {
    std::cout << "Enter BinOp type:" << node.type << std::endl;
    auto lhs = evalExpr(*_ast->getNode(node.lhs)),
         rhs = evalExpr(*_ast->getNode(node.rhs));
    assert(lhs.type == rhs.type);
    _return(selector<Binary, Int, Float>(lhs.type, node.type, lhs, rhs));
    std::cout << "Exit BinOp type:" << node.type << std::endl;
}

void Interpreter::execute(const Call &node) {
    auto sym_tab = _ast->seekEnv(&const_cast<Call &>(node));
    FuncInfo func_info = sym_tab->func_table.getInfo(node.func_info);

    // if (SyLib::func_table.contains(func_info.name)) {
    //     _return(SyLib::call(func_info.name))
    // }

    const auto &ret_type = func_info.type->getResult();
    const auto &func_decl = *_ast->getNode(func_info.node);
    auto cfd = _executeCF(func_decl);
    assert(cfd.index() == CFDType::CFDReturn);
    auto cfd_ret = get<CFDType::CFDReturn>(cfd);
    if (ret_type == Void_v) {
        assert(cfd_ret == std::nullopt);
        _return(Value());  // undef
    } else if (ret_type.isBasicType()) {
        assert(cfd_ret != std::nullopt);
        auto retval = cfd_ret.value();
        assert(retval.type == &ret_type);
        _return(retval);
    } else {
        assert(false && "Wrong ret type");
    }
}

void Interpreter::execute(const DeclRef &node) {
    auto sym_tab = _ast->seekEnv(&const_cast<DeclRef &>(node));
    VarInfo var_info = sym_tab->var_table.getInfo(node.var_id);
    const auto &var_type = *var_info.type;
    auto val = getContext().getInfo(node.var_id);
    _return(val.toLValue());
}

void Interpreter::execute(const ArrayRef &node) {
    auto sym_tab = _ast->seekEnv(&const_cast<ArrayRef &>(node));
    VarInfo var_info = sym_tab->var_table.getInfo(node.var_id);
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
}

/* ------------ CondExpr ------------ */

void Interpreter::execute(const Not &node) {
    auto val = evalExpr(*_ast->getNode(node.subexpr));
    auto operand = val.get<Int>();
    _return(Value(&Int_v, operand == 0));
}

void Interpreter::execute(const And &node) {
    auto &lhs = evalExpr(*_ast->getNode(node.lhs));
    // assert(lhs.type == rhs.type && lhs.type == Int);
    auto lv = lhs.get<Int>();

    Value::Int_t ret;

    assert(0 <= lv && lv <= 1);
    if (lv != 0) {
        auto &rhs = evalExpr(*_ast->getNode(node.rhs));
        auto rv = rhs.get<Int>();
        assert(0 <= rv && rv <= 1);

        ret = rv;
    } else {
        ret = 0;
    }

    _return(Value{&Int_v, ret});
}

void Interpreter::execute(const Or &node) {
    auto &lhs = evalExpr(*_ast->getNode(node.lhs));
    // assert(lhs.type == rhs.type && lhs.type == Int);
    auto lv = lhs.get<Int>();

    Value::Int_t ret;

    assert(0 <= lv && lv <= 1);
    if (lv == 0) {
        auto &rhs = evalExpr(*_ast->getNode(node.rhs));
        auto rv = rhs.get<Int>();
        assert(0 <= rv && rv <= 1);

        ret = rv;
    } else {
        ret = 0;
    }

    _return(Value{&Int_v, ret});
}

void Interpreter::execute(const Compare &node) {
    auto lhs = evalExpr(*_ast->getNode(node.lhs)),
         rhs = evalExpr(*_ast->getNode(node.rhs));
    assert(lhs.type == rhs.type);
    _return(selector<Comp, Int, Float>(lhs.type, node.type, lhs, rhs));
}

void Interpreter::execute(const ToCond &node) {
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
}

/* ---------- Control Flow ---------- */

void Interpreter::execute(const If &node) {
    auto cond = evalExpr(*_ast->getNode(node.cond));
    assert(cond.type == &Int_v);
    int val = cond.get<Int>();
    assert(0 <= val && val <= 1);
    if (val) {
        auto &iftrue = *_ast->getNode(node.stmts);
        iftrue.execute(this);
    } else {
        // if (node.? != std::nullopt) {
        //     auto &iffalse = *_ast->getNode(node.stmts);
        //     iffalse.execute(this);
        // }
    }
}

void Interpreter::execute(const While &node) {
    auto &condnode = *_ast->getNode(node.cond);
    // assert(dynamic_cast<CondExpr &>(condnode) != nullptr);
    auto &block = *_ast->getNode(node.stmt);
    while (true) {
        auto cond = evalExpr(condnode).get<Int>();
        if (!cond) break;
        if (_isNone()) break;
        auto cfd = _executeCF(block);
        if (cfd.index() == CFDType::CFDBreak) {
            _return(Void_v);
            break;
        } else if (cfd.index() == CFDType::CFDContinue) {
            _return(Void_v);
            continue;
        } else
            break;  // Return
    }
}

void Interpreter::execute(const Break &node) {
    _return(ControlFlowData(node));
}

void Interpreter::execute(const Continue &node) {
    _return(ControlFlowData(node));
}

void Interpreter::execute(const Return &node) {
    if (node.returned) {
        auto retval = evalExpr(*_ast->getNode(node.returned));
        _return(ControlFlowData(retval));
    } else {
        _return(ControlFlowData(std::nullopt));
    }
}

void Interpreter::execute(const Block &node) {
    // New Context
    if (_context_stack.empty()) _context_stack.push(Context());
    else _context_stack.push(Context(_context_stack.top()));

    for (auto stmt : node.stmts) {
        auto &stmt_node = *_ast->getNode(stmt);
        stmt_node.execute(this);
        if (_isNone())
            continue;
        else if (_isValue()) {
            _return_value = Void_v;  // discard ExprNode
            continue;
        } else if (_isCFD()) {
            // Passdown
            break;
        } else
            assert(0 && "bug");
    }

    _context_stack.pop();
}

/* ---------------------------------- */

void Interpreter::execute(const Assign &node) {
    auto lv = evalExpr(*_ast->getNode(node.l_val));
    auto rv = evalExpr(*_ast->getNode(node.r_val));
    lv.assign(rv);
}

void Interpreter::execute(const Empty &node) {
    // empty
}

}  // namespace SysYust::AST::Interpreter