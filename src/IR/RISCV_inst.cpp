//
// Created by LL06p on 24-8-8.
//

#include "IR/RISCV_inst.h"

namespace SysYust::IR {

    const Type * returned_type_for_ir(RiscVInst inst) {

#pragma push_macro("process_inst")
#define process_inst(ins) { \
        using returned_type = RV_Type<ins>::return_type; \
        if constexpr (std::is_same_v<returned_type, i_reg>) { \
            return Type::get(Type::f);\
        } else if constexpr (std::is_same_v<returned_type, f_reg>) { \
            return Type::get(Type::i);              \
        } else if constexpr (std::is_same_v<returned_type, void>) {   \
            return nullptr; \
        } else { \
            __builtin_unreachable();\
        }\
    }\

        switch (inst) {
            case BEGIN_RV64IM: process_inst(BEGIN_RV64IM);;
            case MV: process_inst(MV);;
            case NOP: process_inst(NOP);;
            case NEG: process_inst(NEG);;
            case ADD: process_inst(ADD);;
            case ADDI: process_inst(ADDI);;
            case SUB: process_inst(SUB);;
            case MUL: process_inst(MUL);;
            case DIV: process_inst(DIV);;
            case REM: process_inst(REM);;
            case NEGW: process_inst(NEGW);;
            case AND: process_inst(AND);;
            case ANDI: process_inst(ANDI);;
            case XOR: process_inst(XOR);;
            case XORI: process_inst(XORI);;
            case NOT: process_inst(NOT);;
            case OR: process_inst(OR);;
            case ORI: process_inst(ORI);;
            case SLL: process_inst(SLL);;
            case SLLI: process_inst(SLLI);;
            case SRL: process_inst(SRL);;
            case SRLI: process_inst(SRLI);;
            case SRA: process_inst(SRA);;
            case SRAI: process_inst(SRAI);;
            case BEQ: process_inst(BEQ);;
            case BNE: process_inst(BNE);;
            case SLT: process_inst(SLT);;
            case SLTI: process_inst(SLTI);;
            case AUIPC: process_inst(AUIPC);;
            case J: process_inst(J);;
            case CALL: process_inst(CALL);;
            case TAIL: process_inst(TAIL);;
            case RET: process_inst(RET);;
            case LA: process_inst(LA);;
            case LW: process_inst(LW);;
            case SW: process_inst(SW);;
            case LI: process_inst(LI);;
            case LUI: process_inst(LUI);;
            case END_RV64IM: process_inst(END_RV64IM);;
            case BEGIN_RV64FD: process_inst(BEGIN_RV64FD);;
            case FNEG_S: process_inst(FNEG_S);;
            case FADD_S: process_inst(FADD_S);;
            case FSUB_S: process_inst(FSUB_S);;
            case FMUL_S: process_inst(FMUL_S);;
            case FDIV_S: process_inst(FDIV_S);;

            case FCVT_S_W: process_inst(FCVT_S_W);;
            case FCVT_W_S: process_inst(FCVT_W_S);;

            case FLW: process_inst(FLW);;
            case FSW: process_inst(FSW);;

            case FEQ_S: process_inst(FEQ_S);;
            case FLE_S: process_inst(FLE_S);;
            case FLT_S: process_inst(FLT_S);;

            case FMV_S: process_inst(FMV_S);;
            case FMV_W_X: process_inst(FMV_W_X);;
            case FMV_X_W: process_inst(FMV_X_W);;

            case END_RV64FD: process_inst(END_RV64FD);;

        }

#pragma pop_macro("process_inst")

        return nullptr;
    }

} //IR