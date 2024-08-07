#include "IREmitter/IREmitter.h"

#include <fmt/args.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <cassert>
//#include <compare>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <limits>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include "AST/Env/FuncInfo.h"
#include "AST/Env/SymbolTable.h"
#include "AST/Env/VarInfo.h"
#include "AST/Node/ArrayRef.h"
#include "AST/Node/BinaryOp.h"
#include "AST/Node/Compare.h"
#include "AST/Node/Continue.h"
#include "AST/Node/DeclRef.h"
#include "AST/Node/Expr.h"
#include "AST/Node/FloatLiteral.h"
#include "AST/Node/IntLiteral.h"
#include "AST/Node/ParamDecl.h"
#include "AST/Node/UnaryOp.h"
#include "AST/Node/VarDecl.h"
#include "AST/SyntaxTree.h"
#include "AST/Type/Array.h"
#include "AST/Type/TypeBase.h"
#include "IR/BasicBlock.h"
#include "IR/InitInfo.h"
#include "IR/Instruction.h"
#include "IR/SymbolUtil.h"
#include "IR/TypeUtil.h"
#include "IRPrinter/IRPrinter.h"
#include "IdAllocator.h"
#include "Trace.h"
#include "fmt/core.h"
#include "fmt/format.h"

namespace SysYust::AST::IREmitter {

/* -------------- Debug -------------- */

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
                      __VA_ARGS__ __VA_OPT__(, ) _return_value.toString())
#else

#define enterln(fmt_str, ...)
#define exitln(fmt_str, ...)

#endif

std::string IREmitter::Value::toString() const {
    return isVar() ? fmt::to_string(var()) : fmt::to_string(im());
}

/* ------- Algorithm required ------- */

IREmitter::Value IREmitter::readVar(VarId var_id, IR::BasicBlock *bb) {
    auto [id, type] = var_id;
    bool _isGlobal = false;
    if (!bb) {
        bb = current_block();
        // auto &env = 
        // _isGlobal = env.var_table.getInfo(id).isGlobal;
        _isGlobal = !isLive(id);
    } else _isGlobal = !isLive(id);
    if (!type) type = curEnv().var_table.getInfo(id).type;
    if (_isGlobal) {
        assert(_global_vars.contains(id));
        auto var_info = curEnv().var_table.getInfo(id);
        auto val = _global_vars.getInfo(id);
        if (var_info.isConstant && var_info.type->isBasicType()) {
            println("Read global const var {}", id);
            auto init =
                const_cast<std::unordered_map<IR::var_symbol, IR::InitInfo> &>(
                    ir_code()->all_var_init_value())[val.var()];
            if (init.is_list()) {
                assert(init.list().empty());
                return IR::im_symbol{0};
            } else {
                return init.value();
            }
        } else {
            println("Read global {}", id);
            return val;
        }
    } else if (auto &cache = getContext(bb).read_cache; cache.contains(id)) {
        println("hit {} in {}", id, (void *)bb);
        return cache.getInfo(id);
    } else {
        return readVarRec({id, type}, bb);
    }
}

IREmitter::Value IREmitter::readVarRec(VarId var_id, IR::BasicBlock *bb) {
    auto &ctx = getContext(bb);
    auto &cache = ctx.read_cache;
    auto [id, type] = var_id;

    auto &&idCaptured = id;
    auto &&typeCaptured = type;

    auto val = [&]() {
        if (!ctx.seal) {  // for incomplete cfg pred
            println("hit incomplete");
            auto val = newValue(ptr_arr(corr_type(typeCaptured)));
            ctx.incomplete_args.emplace_back(var_id, val);
            return val;
        } else if (bb->prevBlocks().size() == 1) {
            println("forward in {}", (void *)bb);
            return readVar(var_id, bb->prevBlocks()[0]);
        } else {
            assert(bb->prevBlocks().size() != 0);
            auto placeholder = newValue(ptr_arr(corr_type(typeCaptured)));
            writeVar(idCaptured,
                     placeholder);  // for circular depend (v = phi(1, v))
            addArg(var_id, placeholder, bb);  // fill ba
            println("collect {} in {}", placeholder.toString(), (void *)bb);
            return placeholder;
        }
    }();

    writeVar(id, val);
    return val;
}

void IREmitter::writeVar(NumId var_id, Value val) {
    auto bb = current_block();
    auto &ctx = getContext(bb);
    ctx.read_cache.setInfo(var_id, val);
    println("Write {} = {} in {}", var_id, val.toString(), (void *)bb);
}

void IREmitter::sealBlock(IR::BasicBlock *bb) {
    if (!bb) bb = current_block();
    auto &ctx = getContext(bb);
    for (auto [var_id, val] : ctx.incomplete_args) {
        addArg(var_id, val, bb);
    }
    ctx.incomplete_args.clear();
    ctx.seal = true;
}

std::vector<IR::operant> IREmitter::getCallerArgs(IR::BasicBlock *cur,
                                                  IR::BasicBlock *pred) {
    assert(cur && pred);
    auto term = pred->gateway();
    switch (term.index()) {
        case 0:
            assert(false && "gateway is not emitted!");
            __builtin_unreachable();
        case 1: {  // branch
            auto br = std::get<IR::branch>(term);
            if (pred->nextBlock() == cur) {
                return br.true_args;
            } else if (pred->elseBlock() == cur) {
                return br.false_args;
            } else {
                assert(false && "blocks are disconnected!");
                __builtin_unreachable();
            }
        }
        case 2: {  // jump
            auto jp = std::get<IR::jump>(term);
            assert(pred->nextBlock() == cur);
            return jp.args;
        }
        case 3:  // ret
        default:
            assert(false && "successor should not occur in ret block!");
            __builtin_unreachable();
    }
}

void IREmitter::addArg(VarId var_id, Value val, IR::BasicBlock *bb) {
    assert(bb);
    bb->add_arg(val);
    for (auto pred : bb->prevBlocks()) {
        getCallerArgs(bb, pred).emplace_back(
            static_cast<const IR::operant &>(readVar(var_id, pred)));
    }
}

/* -------------- Enter -------------- */

void IREmitter::emitBlock(IR::BasicBlock *bb, std::function<void()> emitter,
                          bool seal) {
    save_and_entry(bb);

    emitter();

    assert(isTerm());

    pop_block();

    if (seal) sealBlock(bb);
}

IR::Code *IREmitter::enter(SyntaxTree *ast) {
    _ast = ast;
    setup_code();

    enterln("Enter TopLevel");

    StackWapper env(this, *_ast->topEnv());

    auto &main_func_decl = *cast_unwrap<FuncDecl *>(
        _ast->getNode(env->func_table.getInfo(env->getId("main")).node));

    printEnv();

    // emit global variables
    _inGlobalScope = true;
    auto seq = env->var_table.getSequence();
    for (auto gind : seq) {
        auto gvar = env->var_table.getInfo(gind);
        auto var_decl = gvar.decl;
        auto &var_node = *cast_unwrap<VarDecl *>(_ast->getNode(var_decl));
        var_node.execute(this);
    }

    _inGlobalScope = false;
    auto seq_func = env->func_table.getSequence();
    for (auto gind : seq_func) {
        auto gfunc = env->func_table.getInfo(gind);
        if (gfunc.isBuiltin) {
            const auto &ret_type =
                gfunc.type ? gfunc.type->getResult() : Void_v;
            auto func_sym = IR::func_symbol{gfunc.name};
            ir_code()->setup_procedure(func_sym, {corr_type(&ret_type)});
        } else if (gfunc.name != "main") {
            auto func_decl = gfunc.node;
            auto &func_node = *ast->getNode(func_decl);
            func_node.execute(this);
        }
    }

    // emit main
    main_func_decl.execute(this);

    exitln("Exit TopLevel");

    return getCode();
}

/* -------------- Decl -------------- */

IR::im_symbol IREmitter::constevalExpr(Node &node) {
    _inConstCtx = true;
    auto ret = evalExpr(node).im();
    _inConstCtx = false;
    return ret;
}

IR::InitInfo IREmitter::parseInitList(const Type &init_type,
                                      std::vector<HNode> &inits) {
    const std::vector<IR::InitInfo *> def_init{};
    if (init_type.isBasicType()) {
        assert(inits.size() <= 1);
        if (inits.size()) {  // int genshin = 1; or {1} in initlist
            auto val_node = inits[0];
            if (_inGlobalScope) {
                return constevalExpr(*_ast->getNode(val_node));
            } else {
                auto val = evalExpr(*_ast->getNode(val_node));
                return val.isIm() ? IR::InitInfo(val.im()) : val.var();
            }
        } else {  // int genshin;
            return def_init;
        }
    } else {  // list
        auto arr_type_ptr =
            dynamic_cast<Array *>(&const_cast<Type &>(init_type));
        assert(arr_type_ptr && "Not an array");
        auto arr_type = *arr_type_ptr;

        if (!inits.size()) {  // = {};
            return def_init;
        }

        auto &deref_type = const_cast<Type &>(arr_type.index(1));
        long long elem_size = 1;
        if (deref_type.type() == TypeId::Array) {
            auto arr = dynamic_cast<const Array &>(deref_type);
            elem_size = arr.size();
        }

        std::vector<IR::InitInfo *> init_list;
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
                    init_list.emplace_back(
                        new IR::InitInfo(parseInitList(deref_type, bares)));
                    bares.clear();
                }
                init_list.emplace_back(new IR::InitInfo(
                    parseInitList(deref_type, list_ptr->vals)));
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
                init_list.emplace_back(
                    new IR::InitInfo(parseInitList(deref_type, part)));
            }
            if (r) {
                std::vector<HNode> part(bares.begin() + k * elem_size,
                                        bares.begin() + len);
                init_list.emplace_back(
                    new IR::InitInfo(parseInitList(deref_type, part)));
            }
        }

        return init_list;
    }
}

void IREmitter::printInitList(IR::InitInfo &inits, int ind) {
    if (inits.is_list()) {
        auto list = inits.list();
        if (list.empty()) {
            println("{}", "{}");
        } else {
            auto len = list.size();
            println("|- [{}] = {} elems", ind, len);
            sp.push();
            for (int i = 0; i < len; ++i) {
                printInitList(*list[i], i);
            }
            sp.pop();
        }
    } else {
        auto val = inits.value();
        println("[{}] {}", ind, val);
    }
}

void IREmitter::FillInitListMemory(Type &type, Value arr_ptr,
                                   const IR::InitInfo &inits) {
    static std::vector<Value> indexes;
    if (!inits.is_list()) {  // Value
        auto val = inits.value();
        auto arr_elem_ptr = createInst<IR::indexof>(arr_ptr, indexes);
        createInst<IR::st>(arr_elem_ptr, val);
    } else {
        auto &arr_type = static_cast<Array &>(type);
        auto &elem_type = const_cast<Type &>(arr_type.index(1));
        auto list = inits.list();
        auto len = list.size();
        for (int i = 0; i < len; ++i) {
            indexes.push_back(Value(i));
            FillInitListMemory(elem_type, arr_ptr, *list[i]);
            indexes.pop_back();
        }
    }
}

void IREmitter::execute(const VarDecl &node) {
    enterln("! VarDecl pre seek");

    auto &env = curEnv();

    auto var_id = node.info_id;
    auto var_decl = env.var_table.getInfo(var_id);

    IREMITTER_DBG_REGION({
        auto init_str =
            node.init_expr.has_value()
                ? fmt::format(" = (init_id: {})", node.init_expr.value())
                : "";
        sp.pop();
        println("VarDecl {} : {}{}", var_decl.name, var_decl.type->toString(),
                init_str);
        sp.push();
    })

    if (_inGlobalScope) {
        auto &code = *ir_code();
        Value val;
        val = newValue(IR::Type::get(IR::Type::ptr, corr_type(var_decl.type)));

        if (node.init_expr.has_value()) {
            auto &init_node = *_ast->getNode(node.init_expr.value());
            if (auto list_node = dynamic_cast<List *>(&init_node)) {
                assert(var_decl.type->type() == TypeId::Array);

                auto &var_type =
                    dynamic_cast<Array &>(const_cast<Type &>(*var_decl.type));

                auto init_list = parseInitList(var_type, list_node->vals);

                IREMITTER_DBG_REGION(printInitList(init_list);)

                code.set_var(val, init_list);
            } else if (auto expr_node = dynamic_cast<Expr *>(&init_node)) {
                auto init_val = constevalExpr(*expr_node);
                code.set_var(val, init_val);
            } else {
                assert(false && "Wrong init list");
                __builtin_unreachable();
            }
        } else {
            // value is zero
            code.set_var(val, std::vector<IR::InitInfo *>{});
        }
        _global_vars.setInfo(var_id, val);
        println("Write {} = {} in global", var_id, val.toString());
    } else {
        if (node.init_expr.has_value()) {
            auto &init_node = *_ast->getNode(node.init_expr.value());
            if (auto list_node = dynamic_cast<List *>(&init_node)) {
                assert(var_decl.type->type() == TypeId::Array);

                auto &var_type =
                    dynamic_cast<Array &>(const_cast<Type &>(*var_decl.type));

                auto init_list = parseInitList(var_type, list_node->vals);

                IREMITTER_DBG_REGION(printInitList(init_list);)

                auto val = createInst<IR::alc>(corr_type(&var_type));

                FillInitListMemory(var_type, val, init_list);

                writeVar(var_id, val);
            } else if (auto expr_node = dynamic_cast<Expr *>(&init_node)) {
                Value val = evalExpr(init_node);
                assert(val.type().isBasic());
                assert(val.type() == *corr_type(var_decl.type));

                writeVar(var_id, val);
            } else {
                assert(false && "Wrong init list");
            }
        } else { // int a[2]; / int a;
            if (var_decl.type->isBasicType()) {
                // value is undef
                writeVar(var_id, undef);
            } else {
                auto val = createInst<IR::alc>(corr_type(var_decl.type));
                writeVar(var_id, val);
            }
        }
        setLive(var_id);
    }

    exitln("~VarDecl {}", var_decl.name);
}

void IREmitter::execute(const FuncDecl &node) {
    enterln("FuncDecl");

    auto cur_env = _ast->seekEnv(&node);
    // cur_env->var_table._parent = nullptr;
    StackWapper env(this, *cur_env);

    auto &func_info = env->func_table.getInfo(node.info_id);

    const auto &ret_type = func_info.type->getResult();
    auto func_sym = IR::func_symbol{func_info.name};
    entry_function(func_sym, {corr_type(&ret_type)});
    entry_block();

    enterln("FuncDecl {}", toString(func_info, node.info_id));

    printEnv();

    // Params
    assert(_isNone());
    size_t len = node.param.size();
    auto proc = procedure();
    for (size_t i = 0; i < len; ++i) {
        auto param_node =
            cast_unwrap<ParamDecl *>(_ast->getNode(node.param[i]));
        setLive(param_node->info_id);
        auto var_info = env->var_table.getInfo(param_node->info_id);
        auto corr_ty = ptr_arr(corr_type(var_info.type));
        auto val = newValue(corr_ty);
        proc->add_param(val);
        writeVar(param_node->info_id, val);
    }

    bool is_main = func_info.name == "main";

    // if (is_main) {
    //     auto before_main_sym = IR::func_symbol{"before_main"};
    //     createInst<IR::call>(before_main_sym, std::vector<Value>{});
    // }

    auto &func_body = *_ast->getNode(node.entry_node);
    func_body.execute(this);

    // if (is_main) {
    //     auto after_main_sym = IR::func_symbol{"after_main"};
    //     createInst<IR::call>(after_main_sym, std::vector<Value>{});
    // }

    exitln("~FuncDecl");
}

void IREmitter::execute(const ParamDecl &node) {
    assert(0 && "ParamDecl shouldn't be visited!");
}

/* -------------- Expr -------------- */

void IREmitter::execute(const IntLiteral &node) {
    _return(IR::im_symbol{node.value});
    println("IntLiteral {}", _return_value.toString());
}

void IREmitter::execute(const FloatLiteral &node) {
    _return(IR::im_symbol{node.value});
    println("FloatLiteral {}", _return_value.toString());
}

void IREmitter::execute(const ToInt &node) {
    enterln("ToInt");

    auto &op_node = *_ast->getNode(node.operant);
    auto val = evalExpr(op_node);
    assert(val.type().isFlt());

    if (_inConstCtx || val.isIm()) {
        assert(val.isIm());
        _return(IR::im_symbol(static_cast<int>(val.imf())));
        exitln("~ToInt");
        return;
    }

    _return(createInst<IR::f2i>(val));

    exitln("~ToInt");
}

void IREmitter::execute(const ToFloat &node) {
    enterln("ToFloat");

    auto &op_node = *_ast->getNode(node.operant);
    auto val = evalExpr(op_node);
    assert(val.type().isInt());

    if (_inConstCtx || val.isIm()) {
        assert(val.isIm());
        _return(IR::im_symbol(static_cast<float>(val.imi())));
        exitln("~ToFloat");
        return;
    }

    _return(createInst<IR::i2f>(val));

    exitln("~ToFloat");
}

template <typename T>
T constevalUnaryOp(UnaryOp::OpType type, T val) {
    if (type == UnaryOp::Positive) {
        return val;
    } else {
        return -val;
    }
}

void IREmitter::execute(const UnaryOp &node) {
    enterln("UnaryOp type: {}", toString(node.type));

    Value val = evalExpr(*_ast->getNode(node.subexpr));

    if (_inConstCtx || val.isIm()) {
        assert(val.isIm());
        _return([&]() -> IR::im_symbol {
            if (val.isImI()) {
                return constevalUnaryOp<int>(node.type, val.imi());
            } else {
                return constevalUnaryOp<float>(node.type, val.imf());
            }
        }());

        exitln("~UnaryOp {} {}", toString(node.type), val.toString());
        return;
    }

    if (node.type == UnaryOp::Positive) {
        _return(val);
    } else {
        _return(createInst<IR::neg>(val));
    }

    exitln("~UnaryOp {} {}", toString(node.type), val.toString());
}

template <typename T>
T constevalBinaryOp(BinaryOp::OpType type, T op1, T op2) {
    switch (type) {
        case BinaryOp::Add:
            return op1 + op2;
            break;
        case BinaryOp::Sub:
            return op1 - op2;
            break;
        case BinaryOp::Mul:
            return op1 * op2;
            break;
        case BinaryOp::Div:
            assert(op2 != 0);
            return op1 / op2;
            break;
        case BinaryOp::Mod:
            if constexpr (std::is_same_v<T, int>) {
                assert(op2 != 0);
                return op1 % op2;
            } else {
                assert(false && "float mod!");
            }
            break;
        default:
            assert(false && "Bug");
    }
}

void IREmitter::execute(const BinaryOp &node) {
    enterln("BinaryOp type: {}", toString(node.type));

    auto lhs = evalExpr(*_ast->getNode(node.lhs)),
         rhs = evalExpr(*_ast->getNode(node.rhs));
    assert(lhs.type() == rhs.type());
    assert(!(lhs.type().isFlt() && node.type == BinaryOp::OpType::Mod) &&
           "Mod float occurs!");

    if (_inConstCtx || lhs.isIm() && rhs.isIm()) {
        assert(lhs.isIm() && rhs.isIm());
        _return([&]() -> IR::im_symbol {
            if (lhs.isImI()) {
                return constevalBinaryOp<int>(node.type, lhs.imi(), rhs.imi());
            } else {
                return constevalBinaryOp<float>(node.type, lhs.imf(),
                                                rhs.imf());
            }
        }());

        exitln("~BinaryOp {} {} {}", lhs.toString(), toString(node.type),
               rhs.toString());
        return;
    }

    const IR::instruct_type op_tab[] = {
        IR::add,   // Add
        IR::sub,   // Sub
        IR::mul,   // Mul
        IR::divi,  // Div
        IR::mod,   // Mod
    };
    _return(
        createInstCate<IR::instruct_cate::with_2>(op_tab[node.type], lhs, rhs));

    exitln("~BinaryOp {} {} {}", lhs.toString(), toString(node.type),
           rhs.toString());
}

void IREmitter::execute(const Call &node) {
    auto &env = curEnv();
    auto &func_info = env.func_table.getInfo(node.func_info);
    const auto &ret_type = func_info.type->getResult();

    enterln("Call {}", toString(func_info, node.func_info));

    auto func_sym = IR::func_symbol{func_info.name};

    // Arguments evaluation
    std::vector<Value> arg_vals;
    for (auto arg : node.argumentExpr) {
        auto &arg_node = *_ast->getNode(arg);
        auto arg_val = evalExpr(arg_node);
        arg_vals.push_back(arg_val);
    }

    if (!func_info.isBuiltin) {
        auto &func_decl =
            *cast_unwrap<FuncDecl *>(_ast->getNode(func_info.node));
        assert(func_decl.param.size() == node.argumentExpr.size());
    }

    if (ret_type.type() == TypeId::Void) {
        createInst<IR::call>(func_sym, arg_vals);
    } else {
        _return(createInst<IR::call>(func_sym, arg_vals));
    }

    exitln("~Call {}", func_info.name);
}

void IREmitter::execute(const DeclRef &node) {
    auto &env = curEnv();

    printEnv();

    auto var_info = env.var_table.getInfo(node.var_id);
    bool _isGlobal = !isLive(node.var_id);

    auto val = readVar({node.var_id, nullptr});

    if (val.isIm()) {
        _return(val);
    }

    if (_isGlobal && val.type().isPtr() && val.type().subtype()->isBasic()) {
        _return(createInst<IR::ld>(val));
    } else {
        _return(val);
    }

    println("DeclRef {} -> {} ({})", var_info.name, _return_value.toString(),
            toString(var_info));
}

IR::im_symbol constevalArrayRef(IR::InitInfo &info,
                                std::vector<IREmitter::Value> &index,
                                size_t layer) {
    assert(layer <= index.size());
    if (info.is_list()) {
        auto cur_ind = index[layer].imi();
        if (info.list().size() - 1 >= cur_ind) {
            return constevalArrayRef(*info.list()[cur_ind], index, layer + 1);
        } else
            return 0;
    } else {
        return info.value().im();
    }
}

void IREmitter::execute(const ArrayRef &node) {
    // println("! ArrayRef pre seek");
    auto _in_direct_assign = _toLValue;
    _toLValue = false;
    auto &env = curEnv();
    auto &var_info = env.var_table.getInfo(node.var_id);
    auto var_type = var_info.type;
    bool _isGlobalConstArr = var_info.isConstant && !isLive(node.var_id);

    // getInfoTrace(node.var_id);
    auto arr = readVar({node.var_id, var_info.type});

    println("! ArrayRef indexes");

    std::vector<Value> val_scripts;
    bool _isConstIndex = true;
    for (auto i : node.subscripts) {
        auto &val_node = *_ast->getNode(i);
        auto val = evalExpr(val_node);
        assert(val.type().isInt());
        val_scripts.push_back(val);

        _isConstIndex &= val.isIm();
    }
    assert(val_scripts.size() == node.subscripts.size());

    assert(!(_isGlobalConstArr && _in_direct_assign) &&
           "You can not assign a const-qualified array!");

    if (_isGlobalConstArr && _isConstIndex) {
        auto init_val =
            const_cast<std::unordered_map<IR::var_symbol, IR::InitInfo> &>(
                ir_code()->all_var_init_value())[arr];
        _return(constevalArrayRef(init_val, val_scripts, 0));

        std::string ind_str;
        for (auto val : val_scripts) {
            ind_str += fmt::format("{}, ", val.toString());
        }

        println("ArrayRef constGlobal {}[{}] -> {} ({})", var_info.name,
                ind_str, _return_value.toString(), toString(var_info));
        return;
    }

    auto val = createInst<IR::indexof>(arr, val_scripts);

    if (_in_direct_assign) {
        _return(val);
    } else if (val.type().subtype()->isBasic()) {
        _return(createInst<IR::ld>(val));
    } else {
        _return(val);
    }

    std::string ind_str;
    for (auto val : val_scripts) {
        ind_str += fmt::format("{}, ", val.toString());
    }
    println("ArrayRef {}[{}] -> {} ({})", var_info.name, ind_str,
            _return_value.toString(), toString(var_info));
}

/* ------------ CondExpr ------------ */

void IREmitter::execute(const Not &node) {
    enterln("Not");

    auto val = evalExpr(*_ast->getNode(node.subexpr));
    assert(val.type().isInt());

    if (_inConstCtx || val.isIm()) {
        assert(val.isIm());
        _return(IR::im_symbol(val.imi() == 0));

        exitln("~Not !{}", val.toString());
        return;
    }

    _return(createInst<IR::eq>(val, Value(0)));

    exitln("~Not !{}", val.toString());
}

void IREmitter::execute(const And &node) {
    enterln("And");

    // lv ? rv : 0

    auto lv = evalExpr(*_ast->getNode(node.lhs));

    if (_inConstCtx) {
        auto rv = evalExpr(*_ast->getNode(node.rhs));
        assert(lv.isIm() && rv.isIm());
        _return(IR::im_symbol(lv.imi() && rv.imi()));
        exitln("~And {} && {}", lv.toString(), rv.toString());
        return;
    }

    if (lv.isIm()) {
        if (lv.imi()) {
            _return(evalExpr(*_ast->getNode(node.rhs)));
            exitln("~And const true && {}", _return_value.toString());
        } else {
            _return(IR::im_symbol(0));
            exitln("~And const false && -");
        }
        return;
    }

    auto cont_block = newBasicBlock();
    auto true_block = newBasicBlock();

    std::string rv_str;
    emitBlock(true_block, [&]() {
        auto rv = evalExpr(*_ast->getNode(node.rhs));
        rv_str = rv.toString();
        createInst<IR::jp>(BlockCall(cont_block, {rv}));
    });

    auto val = newValue(IR::Type::get(IRTypeId::i));
    createInst<IR::br>(lv, BlockCall(true_block),
                       BlockCall(cont_block, {Value(0)}));
    auto &cont_args = const_cast<IR::BasicBlock::block_arg_list_type &>(
        cont_block->getArgs());
    cont_args.emplace_back(val.var());

    sealBlock(current_basic_block());
    entry_block(cont_block);

    _return(val);

    exitln("~And {} && {}", lv.toString(), rv_str);
}

void IREmitter::execute(const Or &node) {
    enterln("Or");

    // lv ? 1 : rv

    auto lv = evalExpr(*_ast->getNode(node.lhs));

    if (_inConstCtx) {
        auto rv = evalExpr(*_ast->getNode(node.rhs));
        assert(lv.isIm() || rv.isIm());
        _return(IR::im_symbol(lv.imi() || rv.imi()));
        exitln("~Or {} || {}", lv.toString(), rv.toString());
        return;
    }

    if (lv.isIm()) {
        if (!lv.imi()) {
            _return(evalExpr(*_ast->getNode(node.rhs)));
            exitln("~Or const false || {}", _return_value.toString());
        } else {
            _return(IR::im_symbol(1));
            exitln("~Or const true || -");
        }
        return;
    }

    auto cont_block = newBasicBlock();
    auto false_block = newBasicBlock();

    std::string rv_str;
    emitBlock(false_block, [&]() {
        auto rv = evalExpr(*_ast->getNode(node.rhs));
        rv_str = rv.toString();
        createInst<IR::jp>(BlockCall(cont_block, {rv}));
    });

    auto val = newValue(IR::Type::get(IRTypeId::i));
    createInst<IR::br>(lv, BlockCall(cont_block, {Value(1)}),
                       BlockCall(false_block));
    auto &cont_args = const_cast<IR::BasicBlock::block_arg_list_type &>(
        cont_block->getArgs());
    cont_args.emplace_back(val.var());

    sealBlock(current_basic_block());
    entry_block(cont_block);

    _return(val);

    exitln("~Or {} || {}", lv.toString(), rv_str);
}

template <typename T>
T constevalCompare(Compare::CompareType type, T op1, T op2) {
    switch (type) {
        case Compare::EQ:
            return op1 == op2;
            break;
        case Compare::NE:
            return op1 != op2;
            break;
        case Compare::GT:
            return op1 > op2;
            break;
        case Compare::GE:
            return op1 >= op2;
            break;
        case Compare::LT:
            return op1 < op2;
            break;
        case Compare::LE:
            return op1 <= op2;
            break;
        default:
            assert(false && "Bug");
            __builtin_unreachable();
    }
}

void IREmitter::execute(const Compare &node) {
    enterln("Compare type: {}", toString(node.type));

    auto lhs = evalExpr(*_ast->getNode(node.lhs)),
         rhs = evalExpr(*_ast->getNode(node.rhs));
    assert(lhs.type() == rhs.type());

    if (_inConstCtx || lhs.isIm() && rhs.isIm()) {
        assert(lhs.isIm() && rhs.isIm());
        if (lhs.isImI()) {
            _return(IR::im_symbol(
                constevalCompare<int>(node.type, lhs.imi(), rhs.imi())));
        } else {
            _return(IR::im_symbol(
                constevalCompare<float>(node.type, lhs.imf(), rhs.imf())));
        }
        exitln("~Compare {} {} {}", lhs.toString(), toString(node.type),
               rhs.toString());
        return;
    }

    IR::instruct_type op_tab[] = {
        IR::eq,  // EQ
        IR::ne,  // NE
        IR::gt,  // GT
        IR::ge,  // GE
        IR::lt,  // LT
        IR::le,  // LE
    };
    _return(
        createInstCate<IR::instruct_cate::with_2>(op_tab[node.type], lhs, rhs));

    exitln("~Compare {} {} {}", lhs.toString(), toString(node.type),
           rhs.toString());
}

void IREmitter::execute(const ToCond &node) {
    enterln("ToCond");

    auto val = evalExpr(*_ast->getNode(node.operant));

    if (_inConstCtx || val.isIm()) {
        assert(val.isIm());
        if (val.isImI()) {
            _return(IR::im_symbol(val.imi() != 0));
        } else {
            _return(IR::im_symbol(val.imf() != 0));
        }
        exitln("~ToCond {}", val.toString());
    }

    _return(createInst<IR::ne>(val, Value(0)));

    exitln("~ToCond {}", val.toString());
}

/* ---------- Control Flow ---------- */

void IREmitter::execute(const If &node) {
    enterln("If");

    auto cond = evalExpr(*_ast->getNode(node.cond));

    if (cond.type().isFlt()) {
        enterln("FIXME: ToInt");

        auto val = cond;
        assert(val.type().isFlt());

        if (_inConstCtx || val.isIm()) {
            assert(val.isIm());
            cond = IR::im_symbol(static_cast<int>(val.imf()));
            exitln("FIXME: ~ToInt");
            return;
        }

        cond = createInst<IR::f2i>(val);

        exitln("FIXME: ~ToInt");
    }

    assert(cond.type().isInt());

    if (cond.isIm()) {
        assert(0 <= cond.imi() && cond.imi() <= 1);
        if (cond.imi()) {
            println("! Iftrue:");
            auto &iftrue = *_ast->getNode(node.stmts);
            iftrue.execute(this);
        } else if (node.else_stmt != -1) {
            println("! Iffalse:");
            auto &iffalse = *_ast->getNode(node.else_stmt);
            iffalse.execute(this);
        } else {
            println("! No else_stmt");
        }
        exitln("~If");
        return;
    }

    auto cont_block = newBasicBlock();

    println("! Iftrue:");

    auto then_block = newBasicBlock();
    emitBlock(then_block, [&]() {
        auto &iftrue = *_ast->getNode(node.stmts);
        iftrue.execute(this);
        createInst<IR::jp>(BlockCall(cont_block));
    });

    if (node.else_stmt != -1) {
        println("! Iffalse:");

        auto else_block = newBasicBlock();
        emitBlock(else_block, [&]() {
            auto &iffalse = *_ast->getNode(node.else_stmt);
            iffalse.execute(this);
            createInst<IR::jp>(BlockCall(cont_block));
        });

        createInst<IR::br>(cond, then_block, else_block);
    } else {
        println("! No else_stmt");

        createInst<IR::br>(cond, then_block, cont_block);
    }

    sealBlock(current_basic_block());
    entry_block(cont_block);

    exitln("~If");
}

void IREmitter::execute(const While &node) {
    enterln("While");

    auto &cond_node = *_ast->getNode(node.cond);
    auto &body = *_ast->getNode(node.stmt);

    auto cond_block = newBasicBlock();
    auto body_block = newBasicBlock();
    auto cont_block = newBasicBlock();

    StackWapper<BreakContinue> cfd(this, {cond_block, cont_block});

    emitBlock(
        cond_block,
        [&]() {
            auto cond = evalExpr(cond_node);
            createInst<IR::br>(cond, body_block, cont_block);
        },
        false);

    emitBlock(body_block, [&]() {
        body.execute(this);
        createInst<IR::jp>(BlockCall(cont_block));
    });

    createInst<IR::jp>(cond_block);
    sealBlock(cond_block);
    sealBlock(current_basic_block());
    entry_block(cont_block);

    exitln("~While");
}

void IREmitter::execute(const Break &node) {
    auto [_, cont_block] = curCFD();
    createInst<IR::jp>(cont_block);
    sealBlock(current_block());
    println("Break");
}

void IREmitter::execute(const Continue &node) {
    auto [cond_block, _] = curCFD();
    createInst<IR::jp>(cond_block);
    sealBlock(current_block());
    println("Continue");
}

void IREmitter::execute(const Return &node) {
    enterln("Return");
    if (node.returned != -1) {
        auto ret = evalExpr(*_ast->getNode(node.returned));
        createInst<IR::rt>(ret);
        exitln("~Return");
    } else {
        createInst<IR::rt>();
        exitln("~Return");
    }
}

void IREmitter::execute(const Block &node) {
    enterln("Block");

    StackWapper env(this, *_ast->seekEnv(&node));

    for (auto stmt : node.stmts) {
        auto &stmt_node = *_ast->getNode(stmt);
        stmt_node.execute(this);

        if (!_isNone()) {
            // discard ExprStmt
            _return(undef);
        }
        if (isTerm()) {
            // Control flow transfer
            break;
        }
    }

    exitln("~Block");
}

/* ---------------------------------- */

void IREmitter::execute(const Assign &node) {
    enterln("Assign");

    auto rv = evalExpr(*_ast->getNode(node.r_val));
    if (auto arr_ref = dynamic_cast<ArrayRef *>(_ast->getNode(node.l_val))) {
        _toLValue = true;
        auto lv = evalExpr(*_ast->getNode(node.l_val));
        createInst<IR::st>(lv, rv);
    } else if (auto decl_ref =
                   dynamic_cast<DeclRef *>(_ast->getNode(node.l_val))) {
        auto var_id = decl_ref->var_id;
        auto var_info = curEnv().var_table.getInfo(var_id);
        if (!isLive(var_id)) {
            // is global var!
            auto lv = _global_vars.getInfo(var_id);
            createInst<IR::st>(lv, rv);
        } else {
            // assign SSA Value
            writeVar(var_id, rv);
        }
    } else {
        assert(false && "Assign lhs is not arr or var!");
    }

    exitln("~Assign ? = ?");
}

void IREmitter::execute(const Empty &node) {
    // empty
    println("Empty");
}

}  // namespace SysYust::AST::IREmitter