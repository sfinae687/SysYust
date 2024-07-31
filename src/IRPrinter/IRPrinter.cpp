#include "IRPrinter/IRPrinter.h"

#include <cstddef>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "IR/BasicBlock.h"
#include "IR/Code.h"
#include "IR/ControlFlow.h"
#include "IR/InitInfo.h"
#include "IR/Instruction.h"
#include "IR/Procedure.h"
#include "IR/SymbolUtil.h"
#include "IR/ProcedureContext.h"
#include "fmt/format.h"

namespace SysYust::IR {

/* ------------ TypeUtil ------------ */

std::string format_as(const Type &type) {
    using namespace std::string_literals;
    using Id = Type::TypeId;
    switch (type.id()) {
        case Id::i:
            return "i32"s;
        case Id::f:
            return "f32"s;
        case Id::arr:
            return fmt::format("[{} x {}]", type.data(), *type.subtype());
        case Id::ptr:
            return fmt::format("ptr {}", *type.subtype());
        case Id::dyn:
            return "dyn"s;
        default:
            return "none"s;
    }
}

/* ----------- SymbolUtil ----------- */

std::string format_as(const IR::var_symbol &var) {
    if (var.symbol == "" && var.revision == 0 && var.type == 0) {
        return fmt::format("(Dummy)");
    }
    return fmt::format("({}{}: {})", var.symbol, var.revision, *var.type);
}

std::string format_as(const im_symbol &im) {
    if (im.data.index() == 0) {
        auto int_im = std::get<IR::alias::i32>(im.data);
        return fmt::format("{}i", int_im);
    } else if (im.data.index() == 1) {
        auto float_im = std::get<IR::alias::f32>(im.data);
        return fmt::format("{}f", float_im);
    } else {
        return "undef";
    }
}

std::string format_as(const operant &op) {
    return op.symbol.index() == 0
               ? fmt::to_string(std::get<var_symbol>(op.symbol))
               : fmt::to_string(std::get<im_symbol>(op.symbol));
}

/* --------- InstructionUtil -------- */

std::string format_as(instruct_type type) {
#define c(x)               \
    case instruct_type::x: \
        return #x;
    switch (type) {
        c(INT);

        // 整数
        c(neg);
        c(add);
        c(sub);
        c(mul);
        c(mod);
        c(divi);
        c(rem);
        c(i2f);
        c(i2b);
        c(eq);
        c(ne);
        c(lt);
        c(le);
        c(gt);
        c(ge);

        // 指针操作
        c(indexof);

        // 浮点操作
        c(fneg);
        c(fadd);
        c(fsub);
        c(fmul);
        c(fdiv);
        c(frem);
        c(f2i);
        c(f2b);
        c(feq);
        c(fne);
        c(flt);
        c(fle);
        c(fgt);
        c(fge);

        // 调用
        c(call);

        // 控制流
        c(br);
        c(jp);
        c(rt);

        // 内存
        c(alc);
        c(ld);
        c(st);

        default:
            return "bad instruction type!";
    }
#undef c
}

std::string format_as(instruct_cate type) {
#define c(x)               \
    case instruct_cate::x: \
        return #x;
    switch (type) {
        c(out_of_instruct);
        c(with_2);
        c(with_1);
        c(call);
        c(branch);
        c(jump);
        c(ret);
        c(load);
        c(store);
        c(alloc);
        c(index);

        default:
            return "bad instruction category!";
    }
#undef c
}

std::string format_as(const instruction &inst) {
    switch (inst.index()) {
        case 0: {  // with_2
            auto binary_inst = std::get<0>(inst);
            return fmt::format("{} = {} {} {}", binary_inst.assigned,
                               binary_inst.type, binary_inst.opr1,
                               binary_inst.opr2);
        }
        case 1: {  // with_1
            auto unary_inst = std::get<1>(inst);
            return fmt::format("{} = {} {}", unary_inst.assigned,
                               unary_inst.type, unary_inst.opr);
        }
        case 2: {  // call
            auto call_inst = std::get<2>(inst);
            std::string arg_str;
            for (auto arg : call_inst.args) {  // typo: ture
                arg_str += fmt::to_string(arg);
            }
            auto &ret_var = call_inst.assigned;
            if (ret_var.symbol == "_" && ret_var.revision == 0) {
                return fmt::format("call {}({})", call_inst.func, arg_str);
            } else {
                return fmt::format("{} = call {}({})", ret_var, call_inst.func, arg_str);
            }
        }
        case 3: {  // br
            auto br_inst = std::get<3>(inst);
            std::string true_args_str;
            for (auto arg : br_inst.ture_args) {  // typo: ture
                true_args_str += fmt::to_string(arg);
            }
            std::string false_args_str;
            for (auto arg : br_inst.ture_args) {
                false_args_str += fmt::to_string(arg);
            }
            return fmt::format("br .bb?({}), .bb?({})", true_args_str,
                               false_args_str);
        }
        case 4: {  // jump
            auto jp_inst = std::get<4>(inst);
            std::string true_args_str;
            for (auto arg : jp_inst.args) {  // typo: ture
                true_args_str += fmt::to_string(arg);
            }
            return fmt::format("jump .bb?({})", true_args_str);
        }
        case 5: {  // ret
            auto ret_inst = std::get<5>(inst);
            if (ret_inst.args.has_value()) {
                return fmt::format("ret {}", ret_inst.args.value());
            } else {
                return fmt::format("ret");
            }
        }
        case 6: {  // load
            auto load_inst = std::get<6>(inst);
            return fmt::format(
                "{} = load {}", load_inst.target,
                load_inst.source);  // bug: target should be `assigned`
        }
        case 7: {  // store
            auto store_inst = std::get<7>(inst);
            return fmt::format("store {}, {}", store_inst.target,
                               store_inst.source);
        }
        case 8: {  // alloc
            auto alloc_inst = std::get<8>(inst);
            return fmt::format("{} = alloc {}", alloc_inst.assigned,
                               *alloc_inst.type);
        }
        case 9: {  // indexOf
            auto indexOf_inst = std::get<9>(inst);
            std::string ind_str;
            for (auto ind : indexOf_inst.ind) {
                ind_str += fmt::to_string(ind);
            }
            return fmt::format("{} = index {}, {}", indexOf_inst.assigned,
                               indexOf_inst.arr, ind_str);
        }
        default:
            assert(false && "Unknown inst type");
            __builtin_unreachable();
    }
}

/* ----------- BasicBlock ----------- */

std::string format_as(const BasicBlock &bb) {
    std::string ret;

    ret += fmt::format(".{}: {{", (void *)&bb) + '\n';

    for (auto inst : bb) {
        ret += "    " + fmt::to_string(inst) + "\n";
    }

    ret += "}";

    return ret;
}

/* ----------- ControlFlow ---------- */

std::string format_as(const ControlFlow &cfg) {
    std::string ret;

    std::map<BasicBlock *, std::size_t> deg;
    std::queue<BasicBlock *> topo;
    size_t cnt = 0;
    for (auto &blk_ref : cfg._nodes) {
        auto blk = const_cast<BasicBlock *>(&blk_ref);
        int cur_deg = blk->prevBlocks().size();
        deg[blk] = cur_deg;
        if (cur_deg == 0) topo.push(blk);
        ++cnt;
    }

    while (!topo.empty()) {
        auto blk = topo.front();
        topo.pop();
        ret += "\n" + fmt::to_string(*blk) + "\n";
        if (auto next_blk = blk->nextBlock()) {
            auto it = deg.find(next_blk);
            assert(it != deg.end());
            if (--it->second == 0) {
                deg.erase(it);
                topo.push(next_blk);
            }
        }
        if (auto else_blk = blk->elseBlock()) {
            assert(blk->nextBlock());
            auto it = deg.find(else_blk);
            assert(it != deg.end());
            if (--it->second == 0) {
                deg.erase(it);
                topo.push(else_blk);
            }
        }
    }

    return ret;
}

/* -------------- Code -------------- */

std::string format_as(const InitInfo &init) {
    if (init.is_list()) {
        std::string ret;
        for (auto i : init.list()) {
            ret += fmt::to_string(*i) + ", ";
        }
        return fmt::format("{{{}}}", ret);
    } else {
        return fmt::to_string(init.value());
    }
}

std::string format_as(const func_symbol &func) {
    return func.symbol;
}

std::string format_as(const Procedure &proc) {
    std::string ret;

    std::string params;
    for (auto param : proc.context()->param_var_set) {
        params += fmt::to_string(param) + ", ";
    }

    ret += fmt::format("{}({}) {{\n", proc.name(), params);

    ret += fmt::to_string(const_cast<Procedure &>(proc).getGraph()) + "\n";

    ret += "}";
    return ret;
}

std::string format_as(const Code &code) {
    std::string ret;
    ret += "// Global Variables:\n";
    auto &inits = const_cast<std::unordered_map<var_symbol, InitInfo> &>(code.global_var_value);
    for (auto gvar : code.context.global_vars()) {
        auto init = inits[gvar];
        ret += fmt::format("{} = {}", gvar, init) + '\n';
    }

    ret += "// Functions:\n";

    for (auto gfunc : code.procedures()) {
        ret += fmt::to_string(gfunc) + "\n";
    }

    return ret;
}

}  // namespace SysYust::IR