#include "InstSel/InstSel.h"

#include <bit>
#include <cassert>
#include <optional>
#include <type_traits>

#include "IR/BasicBlock.h"
#include "IR/Code.h"
#include "IR/Instruction.h"
#include "IR/RISCV_inst.h"
#include "IR/SymbolUtil.h"

namespace SysYust::IR {

auto isVar = [](const auto &op) {
    return std::is_same_v<var_symbol, decltype(op)>;
};

auto isIm = [](const auto &op) {
    return std::is_same_v<im_symbol, decltype(op)>;
};

auto imi = [](const auto &op) {
    constexpr auto is_im = std::is_same_v<im_symbol, decltype(op)>;
    assert(is_im && "should be im_symbol!");
    if constexpr (is_im) {
        assert(op.data.index() == 0 && "should be i!");
        return std::get<int>(op.data);
    } else
        return 0;
};

auto imf = [](const auto &op) {
    constexpr auto is_im = std::is_same_v<im_symbol, decltype(op)>;
    assert(is_im && "should be im_symbol!");
    if constexpr (is_im) {
        assert(op.data.index() == 1 && "should be f!");
        return std::get<float>(op.data);
    } else
        return .0f;
};

instruction InstSel::sel_with_1(compute_with_1 inst) {
    auto op = inst.opr;
    switch (inst.type) {
        case neg:
            return Inst<NEGW>(op);
        case fneg:
            return Inst<FNEG_S>(op);
        case f2i:
            return Inst<FCVT_S_W>(op);
        case i2f:
            return Inst<FCVT_W_S>(op);
        default:
            assert(false && "unhandled unary inst type!");
            __builtin_unreachable();
    }
}

instruction InstSel::emitADDI(var_symbol op1, int op2) {
    if (-2048 <= op2 && op2 <= 2047) {
        return Inst<ADDI>(op1, op2);
    } else if (-4096 <= op2 && op2 <= 4094) {
        auto val = createInst<ADDI>(op1, (int)ceil(op2 / 2.0));
        return Inst<ADDI>(val, op2 / 2);
    } else {
        auto imm = createInst<LI>(op2);
        return Inst<ADD>(op1, imm);
    }
}

var_symbol InstSel::ldfimm(operant op) {
    auto imm = op.im();
    auto val = imm | imf;
    int equ_imm = std::bit_cast<int>(val);
    auto imm1 = createInst<LW>(equ_imm);
    return createInst<FMV_X_W>(imm1);
}

instruction InstSel::sel_with_2(compute_with_2 inst) {
    auto op1 = inst.opr1, op2 = inst.opr2;
    assert(op1 | isVar || op2 | isVar);
    switch (inst.type) {
        // i arith
        case add:
            if (op1 | isIm) std::swap(op1, op2);
            if (op2 | isIm)
                return emitADDI(op1.var(), op2 | imi);
            else
                return Inst<ADD>(op1, op2);
        case sub:
            if (op1 | isIm) std::swap(op1, op2);
            if (op2 | isIm)
                return emitADDI(op1.var(), -(op2 | imi));
            else
                return Inst<SUB>(op1, op2);
        case mul:
            if (op1 | isIm) std::swap(op1, op2);
            if (op2 | isIm) {
                auto val = op2 | imi;
                bool sgn = false;
                if (val < 0) {
                    val = -val;
                    sgn = true;
                }

                if (__builtin_popcount(val) == 1) {
                    auto shift = std::__lg(val);
                    if (sgn) {
                        auto val = createInst<SLLI>(op1, shift);
                        return Inst<NEGW>(val);
                    } else {
                        return Inst<SLLI>(op1, shift);
                    }
                }

                auto imm = createInst<LI>(op2);
                return Inst<MUL>(op1, imm);
            } else
                return Inst<MUL>(op1, op2);
        case divi:
            // todo Strength reduction
            if (op1 | isIm) op1 = createInst<LI>(op1);
            if (op2 | isIm) op2 = createInst<LI>(op2);
            return Inst<DIV>(op1, op2);
        case rem:
            // todo Strength reduction
            if (op1 | isIm) op1 = createInst<LI>(op1);
            if (op2 | isIm) op2 = createInst<LI>(op2);
            return Inst<REM>(op1, op2);

        // cmp
        case eq: {
            if (op1 | isIm) std::swap(op1, op2);
            if (op2 | isIm) op2 = createInst<LI>(op2);
            auto val = createInst<XOR>(op1, op2);
            return Inst<SEQZ>(val);
        }
        case ne: {
            if (op1 | isIm) std::swap(op1, op2);
            if (op2 | isIm) op2 = createInst<LI>(op2);
            auto val = createInst<XOR>(op1, op2);
            return Inst<SNEZ>(val);
        }
        case lt:
            if (op1 | isIm) op1 = createInst<LI>(op1);
            if (op2 | isIm) op2 = createInst<LI>(op2);
            return Inst<SLT>(op1, op2);
        case le: {
            if (op1 | isIm) op1 = createInst<LI>(op1);
            if (op2 | isIm) op2 = createInst<LI>(op2);

            // op1 <= op2 <=> !(op2 < op1)
            auto val = createInst<SLT>(op2, op1);
            return Inst<XORI>(val, im_symbol(1));
        }
        case gt:
            if (op1 | isIm) op1 = createInst<LI>(op1);
            if (op2 | isIm) op2 = createInst<LI>(op2);
            return Inst<SLT>(op2, op1);
        case ge: {
            if (op1 | isIm) op1 = createInst<LI>(op1);
            if (op2 | isIm) op2 = createInst<LI>(op2);

            // op1 >= op2 <=> !(op1 < op2)
            auto val = createInst<SLT>(op1, op2);
            return Inst<XORI>(val, 1);
        }

        // f arith
        case fadd:
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            return Inst<FADD_S>(op1, op2);
        case fsub:
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            return Inst<FSUB_S>(op1, op2);
        case fmul:
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            return Inst<FMUL_S>(op1, op2);
        case fdiv:
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            return Inst<FDIV_S>(op1, op2);
        // no frem

        // f cmp
        case feq:
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            return Inst<FEQ_S>(op1, op2);
        case fne: {
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            auto val = createInst<FEQ_S>(op1, op2);
            return Inst<XORI>(val, 1);
        }
        case flt:
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            return Inst<FLT_S>(op1, op2);
        case fle: {
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);

            // op1 <= op2 <=> !(op2 < op1)
            auto val = createInst<FLT_S>(op2, op1);
            return Inst<XORI>(val, 1);
        }
        case fgt:
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);
            return Inst<FLT_S>(op2, op1);
        case fge: {
            if (op1 | isIm) op1 = ldfimm(op1);
            if (op2 | isIm) op2 = ldfimm(op2);

            // op1 >= op2 <=> !(op1 < op2)
            auto val = createInst<FLT_S>(op1, op2);
            return Inst<XORI>(val, 1);
        }
        // f2b? (as op != 0)
        default:
            assert(false && "unhandled unary inst type!");
            __builtin_unreachable();
    }
}

instruction InstSel::sel_load(load inst) {
    return Inst<LW>(inst.source);
}

instruction InstSel::sel_store(store inst) {
    return Inst<SW>(inst.target, inst.source);
}

instruction InstSel::sel_index(indexOf inst) {
    auto val = inst.arr;
    auto type = val.type->subtype();  // deref

    int len = inst.ind.size();
    assert(len >= 1);
    for (int i = 0; i < len; ++i) {
        auto ind = inst.ind[i];
        type = type->subtype();
        auto imm = createInst<LI>(type->size());
        auto val1 = createInst<MUL>(imm, ind);
        if (i == len - 1) {
            return Inst<ADD>(val, val1);
        } else {
            val = createInst<ADD>(val, val1);
        }
    }
    __builtin_unreachable();
}

Code *InstSel::run(Code *code) {
    for (auto &proc : code->procedures()) {
        _cur_proc = &proc;
        for (auto bb : proc.getGraph().all_nodes()) {
            _curBB = &bb;
            for (auto inst_iter = bb.begin(); inst_iter != bb.end();
                 ++inst_iter) {
                auto inst = *inst_iter;
                set_insert_point(inst_iter);
                auto nInst = std::visit(
                    [&](auto &&inst) -> std::optional<instruction> {
                        using ty = std::decay_t<decltype(inst)>;
                        if constexpr (std::is_same_v<ty, compute_with_1>) {
                           return sel_with_1(inst);
                        } else if constexpr (std::is_same_v<ty,
                                                            compute_with_2>) {
                            return sel_with_2(inst);
                        } else if constexpr (std::is_same_v<ty,
                                                            call_instruct>) {
                            return std::nullopt;
                        } else if constexpr (std::is_same_v<ty, branch>) {
                            return std::nullopt;
                        } else if constexpr (std::is_same_v<ty, jump>) {
                            return std::nullopt;
                        } else if constexpr (std::is_same_v<ty, ret>) {
                            return std::nullopt;
                        } else if constexpr (std::is_same_v<ty, load>) {
                            return sel_load(inst);
                        } else if constexpr (std::is_same_v<ty, store>) {
                            return sel_store(inst);
                        } else if constexpr (std::is_same_v<ty, alloc>) {
                            return std::nullopt;
                        } else if constexpr (std::is_same_v<ty, indexOf>) {
                            return sel_index(inst);
                        } else {
                            assert(false && "Unknown inst type");
                            __builtin_unreachable();
                        }
                    },
                    inst);
                if (nInst) proc.retrack_inst(inst_iter, nInst.value());
            }
        }
    }
    return code;
}

}  // namespace SysYust::IR