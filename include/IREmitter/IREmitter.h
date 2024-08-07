/// SysYust AST 的解释器，用于验证 前端 正确性，另给一点信心（

#ifndef SYSYUST_IREMITTER_H
#define SYSYUST_IREMITTER_H

#include <cassert>
#include <climits>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "AST/Env/FuncInfo.h"
#include "AST/Env/SymbolTable.h"
#include "AST/Env/VarInfo.h"
#include "AST/Node/Break.h"
#include "AST/SyntaxTree.h"
#include "AST/Type/Array.h"
#include "AST/Type/Pointer.h"
#include "AST/Type/TypeBase.h"
#include "IR/BasicBlock.h"
#include "IR/CodeUtil.h"
#include "IR/InitInfo.h"
#include "IR/Instruction.h"
#include "IR/SymbolUtil.h"
#include "IR/TypeUtil.h"
#include "IRPrinter/IRPrinter.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "utility/IdAllocator.h"

namespace SysYust::AST::IREmitter {

// For Debug

#define IREMITTER_DEBUG
#define USE_PRINTLN

#ifdef IREMITTER_DEBUG

#define IREMITTER_DBG_REGION(x) {x}
#define IREMITTER_DBG_DECL(x) x

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

#else

#define IREMITTER_DBG_REGION(x)

#ifdef USE_PRINTLN
#undef USE_PRINTLN
#endif

#endif

#ifdef USE_PRINTLN
#if 1  // print to cout
#define println(fmt_str, ...)                                           \
    (std::cout << sp << fmt::format(fmt_str __VA_OPT__(, ) __VA_ARGS__) \
               << std::endl)
#else  // print to log_string
#define println(fmt_str, ...)                                          \
    (log_data << sp << fmt::format(fmt_str __VA_OPT__(, ) __VA_ARGS__) \
              << std::endl)
#endif
#else
#define println(fmt_str, ...)
#endif

// End Debug

using IRTypeId = IR::Type::TypeId;

class IREmitter : public NodeExecutorBase, public IR::CodeBuildMixin {
   public:
    IREMITTER_DBG_DECL(std::stringstream log_data;)

    class Value {
       public:
        Value() : val(undef) {};
        Value(IR::im_symbol::flag f) : val(f) {};
        Value(IR::im_symbol im) : val(im) {};
        Value(IR::var_symbol var)
            : val(std::make_shared<IR::var_symbol>(var)) {};
        Value(IR::operant op) {
            if (op.symbol.index() == 0) {
                new(this) Value(std::move(op.var()));
            } else {
                new(this) Value( op.im());
            }
        };

        std::variant<std::shared_ptr<IR::var_symbol>, IR::im_symbol> val;

        bool isVar() const {
            return val.index() == 0;
        }

        bool isIm() const {
            return val.index() == 1;
        }

        auto var() const {
            assert(isVar());  // must be var_symbol
            return *std::get<0>(val);
        }

        auto im() const {
            assert(isIm());
            return std::get<1>(val);
        }

        auto isImI() {
            assert(isIm());
            return im().data.index() == 0;
        }

        auto imf() {
            assert(isIm());
            return std::get<float>(im().data);
        }

        auto imi() {
            assert(isIm());
            return std::get<int>(im().data);
        }

        bool isUndef() const {
            return isIm() && im().data.index() == 2;
        }

        operator IR::operant() const {
            return isVar() ? IR::operant(var()) : im();
        }

        operator IR::var_symbol() const {
            return var();
        }

        auto &type() const {
            return isVar() ? *var().type : *im().type;
        }

        std::string toString() const;
    };
    inline static constexpr auto undef =  IR::im_symbol::undef;

    class BlockCall {
       public:
        BlockCall(IR::BasicBlock *bb) : bb(bb), args{} {
        }
        BlockCall(IR::BasicBlock *bb, std::vector<Value> args)
            : bb(bb), args(std::move(args)) {
        }
        IR::BasicBlock *bb;
        std::vector<Value> args;
    };

    using VarId = std::pair<NumId, const AST::Type *>;

    class Context {
       public:
        std::vector<std::pair<VarId, Value>> incomplete_args;
        std::vector<std::pair<Value, Value>> arg_witness;
        SymbolTable<Value> read_cache;
        bool seal = false;
    };
    using ContextTable = std::unordered_map<IR::BasicBlock *, Context>;

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

    void emitBlock(IR::BasicBlock *bb, std::function<void()> emitter, bool seal = true);

    template <typename T, typename... Args>
    void reconstruct(T &v, Args... args) {
        std::destroy_at(std::addressof(v));
        new(std::addressof(v)) T(std::forward<Args>(args)...);
    }

    [[nodiscard]] IR::im_symbol constevalExpr(Node &node);

    [[nodiscard]] Value evalExpr(Node &node) {
        // assert node in {IntLiteral, FloatLiteral, Call, UnaryOp, BinaryOp,
        // DeclRef, ArrayRef, ToInt, ToFloat}

        node.execute(this);
        assert(!_isNone());
        auto val = std::move(_return_value);
        reconstruct(_return_value, undef);

        return val;
    }

    IR::Code *enter(SyntaxTree *ast);

    using BreakContinue = std::pair<IR::BasicBlock *, IR::BasicBlock *>;

   private:
    bool _inGlobalScope = true;
    bool _toLValue = false;
    bool _inConstCtx = false;
    Value _return_value = undef;
    ContextTable _ctx_tab;
    SymbolTable<Value> _global_vars;
    std::stack<Env> _env_stack;
    std::stack<SymbolTable<std::monostate>> _lives_stack;
    std::stack<BreakContinue> _cfd_stack;
    SyntaxTree *_ast;

    Context &getContext(IR::BasicBlock *bb) {
        assert(bb);
        return _ctx_tab[bb];
    }

    friend class StackWapper;

    template <typename ElemParam>
    class StackWapper {
       public:
        using Elem = std::remove_cvref_t<ElemParam>;
        IREmitter *emitter;
        std::stack<Elem> *stack;
        StackWapper() = delete;
        StackWapper(IREmitter *emitter, ElemParam elem) : emitter(emitter) {
            if constexpr (std::is_same_v<Elem, Env>) {
                stack = &emitter->_env_stack;
                if (!emitter->_inGlobalScope) {
                    if (!emitter->_lives_stack.empty())
                        emitter->_lives_stack.push(SymbolTable<std::monostate>(
                            emitter->_lives_stack.top()));
                    else
                        emitter->_lives_stack.push({});
                }
            } else if constexpr (std::is_same_v<Elem, BreakContinue>) {
                stack = &emitter->_cfd_stack;
            }
            stack->push(std::forward<ElemParam>(elem));
        }

        ~StackWapper() {
            assert(!stack->empty());
            stack->pop();
            if constexpr (std::is_same_v<Elem, Env>) {
                if (!emitter->_inGlobalScope &&
                    !emitter->_lives_stack.empty()) {
                    emitter->_lives_stack.pop();
                }
            }
        }

        Elem *operator->() {
            assert(!stack->empty());
            return &stack->top();
        }
    };

    auto &curEnv() {
        assert(!_env_stack.empty());
        return _env_stack.top();
    }

    auto &curCFD() {
        assert(!_cfd_stack.empty());
        return _cfd_stack.top();
    }

    void setLive(NumId var_id) {
        assert(!_lives_stack.empty());
        _lives_stack.top().setInfo(var_id, std::monostate{});
    }

    bool isLive(NumId var_id) {
        if (_lives_stack.empty()) {
            return false;
        } else {
            return _lives_stack.top().contains(var_id);
        }
    }

    bool isTerm() {
        auto blk = current_block();
        if (blk->begin() == blk->end()) return false;
        auto term = current_block()->back();
        return IR::is_gateway(static_cast<IR::instruct_cate>(term.index()));
    }

    // void printCtx() {
    //     println("! Ctx layer {}", _context_stack.size());
    //     bool no_entry = 1;
    //     for (auto gvar : getContext()) {
    //         no_entry = 0;
    //         auto id = gvar.first;
    //         auto name = curEnv().var_table.getInfo(id).name;
    //         auto val = gvar.second;
    //         println("● {} \t= {} \t(id: {})", name, val.toString(), id);
    //     }
    //     if (no_entry) println("x No entry.");
    // }

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
        auto isGlobal = var_info.isGlobal;
        return fmt::format(
            "{} \t: {} \t({}decl_id: {}, const? {}, param? {}, global? {})",
            name, type->toString(), id_str, decl, isConstant, isParam,
            isGlobal);
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

    bool _isNone() const {
        return _return_value.isUndef();
    }

    void _return(Value ret) {
        reconstruct(_return_value, std::move(ret));
    }

    IR::InitInfo parseInitList(const Type &init_type,
                               std::vector<HNode> &inits);
    void printInitList(IR::InitInfo &inits, int ind = 0);

    void FillInitListMemory(Type &type, Value arr_ptr,
                            const IR::InitInfo &inits);

    template <typename To, typename From>
    std::vector<To> vec_cast(std::vector<From> val) {
        assert(false && "No impl!");
    }

    template <typename To, typename From, typename Match>
    auto try_cast(Match &&castee) {
        if constexpr (std::is_same_v<std::vector<From>, Match>) {
            return std::move(vec_cast<IR::operant, Value>(castee));
        } else {
            return std::move(castee);
        }
    }

    template <IR::instruct_type T, typename... Args>
    Value createInstImpl(Args... args) {
        println("createInst {}", T);
        constexpr auto ct = IR::cate(T);
        if constexpr (ct == IR::instruct_cate::with_1 ||
                      ct == IR::instruct_cate::with_2 ||
                      ct == IR::instruct_cate::index ||
                      ct == IR::instruct_cate::alloc ||
                      ct == IR::instruct_cate::load ||
                      ct == IR::instruct_cate::call) {
            auto inst_ctx = auto_inst<T>(
                try_cast<IR::operant, Value>(std::forward<Args>(args))...);
            current_block()->push(*inst_ctx);
            if constexpr (T == IR::ld) {
                auto ld = dynamic_cast<IR::load *>(&*inst_ctx);
                return Value(ld->target);
            } else if constexpr (T == IR::call) {
                auto call = dynamic_cast<IR::call_instruct *>(&*inst_ctx);
                return Value(call->assigned);
            } else {
                return Value(inst_ctx->assigned);
            }
        } else {
            // no assigned
            auto inst = *auto_inst<T>(
                try_cast<IR::operant, Value>(std::forward<Args>(args))...);
            current_block()->push(inst);
            return undef;
        }
    }

    template <IR::instruct_type T, typename... Args>
    Value createInst(Args... args) {
        if constexpr (T == IR::br) {
            return [&](Value cond, BlockCall t, BlockCall f) {
                setNext(t.bb);
                setElse(f.bb);
                return createInstImpl<T>(cond, std::move(t.args),
                                         std::move(f.args));
            }(args...);
        } else if constexpr (T == IR::jp) {
            return [&](BlockCall t) {
                setNext(t.bb);
                return createInstImpl<T>(std::move(t.args));
            }(args...);
        } else if constexpr (T == IR::indexof) {
            return [&](Value arr_ptr, std::vector<Value> indexes) {
                return createInstImpl<T>(arr_ptr, std::move(indexes));
            }(args...);
        } else {
            return createInstImpl<T>(std::forward<Args>(args)...);
        }
    }

    template <IR::instruct_type... id, typename... Args>
    [[nodiscard]] Value selectInstImpl(IR::instruct_type type, Args... args) {
        Value ret;
        bool exec =
            ((id == type &&
              (reconstruct(ret, createInst<id>(std::forward<Args>(args)...)),
               true)) ||
             ...);
        // assert(exec && "Inst not exists!");
        return ret;
    }

    template <IR::instruct_cate t, int cnt, int st, int... list,
              typename... Args,
              typename std::enable_if_t<cnt == -1> * = nullptr>
    auto RangeFiliter(IR::instruct_type type, Args... args) {
        return selectInstImpl<static_cast<IR::instruct_type>(st + list)...>(
            type, std::forward<Args>(args)...);
    }

    template <IR::instruct_cate t, int cnt, int st, int... list,
              typename... Args,
              typename std::enable_if_t<cnt != -1> * = nullptr>
    auto RangeFiliter(IR::instruct_type type, Args... args) {
        if constexpr (constexpr auto cat =
                          IR::cate(static_cast<IR::instruct_type>(st + cnt));
                      cat == t) {
            return RangeFiliter<t, cnt - 1, st, cnt, list...>(
                type, std::forward<Args>(args)...);
        } else {
            return RangeFiliter<t, cnt - 1, st, list...>(
                type, std::forward<Args>(args)...);
        }
    }

    template <IR::instruct_cate t, typename... Args>
    Value createInstCate(IR::instruct_type type, Args... args) {
        return RangeFiliter<t, IR::st - IR::neg, IR::neg>(
            type, std::forward<Args>(args)...);
    }

    Value newValue(const IR::Type *type) {
        static long long global_conter = 0;
        if (_inGlobalScope) {
            auto global_ctx = global_context();
            auto val = IR::var_symbol{"@", static_cast<size_t>(++global_conter), type};
            Value ret(val);
            return ret;
        } else {
            auto proc_ctx = procedure_context();
            auto val = proc_ctx->nextSymbol();
            return Value({val.symbol, val.revision, type});
        }
    }

    void writeVar(NumId var_id, Value val);
    Value readVar(VarId var_id, IR::BasicBlock *bb = nullptr);
    Value readVarRec(VarId var_id, IR::BasicBlock *bb);
    void sealBlock(IR::BasicBlock *bb);
    std::vector<IR::operant> getCallerArgs(IR::BasicBlock *cur,
                                           IR::BasicBlock *pred);
    void addArg(VarId var_id, Value val, IR::BasicBlock *bb);

    /// wrap arr by ptr (ptr [3 x [2 x i32]])
    const IR::Type* ptr_arr(const IR::Type *ty) {
        assert(!ty->isPtr());
        if (ty->isArr()) ty = IR::Type::get(IR::Type::ptr, ty);
        return ty;
    }
};

template <>
inline std::vector<IR::operant>
IREmitter::vec_cast<IR::operant, IREmitter::Value>(std::vector<Value> val) {
    std::vector<IR::operant> args;
    for (auto i : val) args.push_back(Value(i));
    return args;
}

template <typename To, typename From>
To cast_unwrap(From &&castee) {
    if (auto cast = dynamic_cast<To>(castee)) {
        return std::move(cast);
    } else {
        assert(false && "Cast Error");
    }
}

inline const IR::Type *corr_type(const Type *type) {
    if (type->type() == TypeId::Int)
        return IR::Type::get(IR::Type::i);
    else if (type->type() == TypeId::Float)
        return IR::Type::get(IR::Type::f);
    else if (type->type() == TypeId::Void)
        return IR::Type::get(IR::Type::none);
    else if (type->type() == TypeId::Array) {
        auto arr_ty = cast_unwrap<const Array *>(type);
        auto sub_arr = &arr_ty->index(1);
        return IR::Type::get(IRTypeId::arr, corr_type(sub_arr),
                             arr_ty->getExtent(0));
    } else if (type->type() == TypeId::Pointer) {
        auto ptr_ty = cast_unwrap<const Pointer *>(type);
        auto sub_ty = &ptr_ty->index(1);
        return IR::Type::get(IRTypeId::arr, corr_type(sub_ty), 0);
    }
    assert(false && "none corr_type");
}

}  // namespace SysYust::AST::IREmitter

#endif  // SYSYUST_IREMITTER_H