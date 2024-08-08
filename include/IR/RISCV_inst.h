//
// Created by LL06p on 24-8-8.
//

#ifndef SYSYUST_RISCV_INST_H
#define SYSYUST_RISCV_INST_H

#include <cstdarg>
#include <cassert>
#include <type_traits>
#include <utility>
#include <string>
#include <variant>
#include <array>
#include <any>


namespace SysYust::IR {

    // 用来描述类型信息的占位类型

    /**
     * @brief 通用寄存器占位类型兼值类型
     */
    struct i_reg {
        explicit i_reg(int no)
            : no(no)
        {
            assert(0 <= no && no < 32);
        }
        [[nodiscard]] std::string full() const {
            return "x" + std::to_string(no);
        }

        friend bool operator== (const i_reg &lhs, const i_reg &rhs) = default;
        friend bool operator< (const i_reg &lhs, const i_reg &rhs) {
            return lhs.no < rhs.no;
        }

        int no;
    };

    /**
     * @brief 浮点寄存器占位类型兼值类型
     */
    struct f_reg {
        explicit f_reg(int no)
            : no(no)
        {
            assert(0 <= no && no <= 32);
        }
        [[nodiscard]] std::string full() const {
            return "f" + std::to_string(no);
        }

        friend bool operator== (const f_reg &lhs, const f_reg &rhs) = default;
        friend bool operator< (const f_reg &lhs, const f_reg &rhs) {
            return lhs.no < rhs.no;
        }

        int no;
    };

    /**
     * @brief 程序内存位置符号的占位类型
     */
    struct symbol_arg : public std::string {};

//    inline constexpr i_reg_t i_reg;
//    inline constexpr f_reg_t f_reg;

    /**
     * @brief 指定位宽的有符号整数类型
     */
    template <std::size_t N>
    struct i_bits_im {
        struct is_riscv_arg{};
        int value : N;
    };
    /**
     * @brief 指定位宽的无符号整数类型
     */
     template <std::size_t N>
     struct u_bits_im {
         struct is_riscv_arg{};
         unsigned value : N;
     };

    template <typename T>
    concept RiscVInstRt = std::is_same_v<T, i_reg> || std::is_same_v<T, f_reg> || std::is_same_v<T, void>;

    using RV_Returned_Value = std::variant<std::monostate, i_reg, f_reg>;

    template <typename T>
    concept RiscVInstArg = std::is_same_v<T, i_reg>
    || std::is_same_v<T, f_reg>
    || std::is_same_v<T, symbol_arg>
    || requires {typename T::is_riscv_arg;};

    using RV_Argument_Value = std::any;

    enum RiscVInst {
        BEGIN_RV64IM,
        MV,
        NOP,

        // 64:
        NEG,
        ADD,
        ADDI,
        SUB,
        MUL,
        DIV,
        REM,

        // 32:
        NEGW,

        // BITWISE 按位逻辑运算
        AND,
        ANDI,
        XOR,
        XORI,
        NOT,
        OR,
        ORI,

        // SHIFT 移位指令
        // S{L/R}{L/A} 第一个 {L/R} 表示左右，第个 {L/A}
        // （LOGICAL/ARITHMETIC）表示逻辑和算数（算数保持符号） 没有 SLA 是因为它和
        // SLL 是等价的
        SLL,
        SLLI,
        SRL,
        SRLI,
        SRA,
        SRAI,

        // BRANCH
        BEQ,
        // BGE,
        // BGT,
        // BLE,
        // BLT,
        BNE,

        // CMP
        // 无符号比较跟有符号一样
        // 没有 SEQ, 它等价于 XOR A, A, B; SEQZ DST, A
        SLT,
        SLTI,

        // 处理 PC 高位偏移
        /**
         * 在 RISC-V 里，由于指令长度的问题（64
         * 位指令可能存有多个参数/立即数，指令中提供给立即数的长度有限），偏移有时无法完全加载（比如
         * 63 位长的偏移），这时要分为高位和低位分开加载。 比如 JALR
         * DST，OFFSET(SRC) OFFSET 只能是 12 位立即数
         * 在加载地址类偏移时，理想的状况是 DST = PC + OFFSET 得到偏移后的地址
         * 加载高位和低位情况时 DST = PC + OFFSETHI + OFFSETLO (OFFSET = OFFSETHI +
         * OFFSETLO) 可以使用 DST1 = PC + OFFSETHI; JALR DST, OFFSETLO(DST1) 来解决
         * 但是由于 RISC-V 的设计，你不能直接访问 PC
         * 那么（PC + OFFSETHI）部分就产生了一个指令 AUIPC（ADD UI TO PC）
         * AUIPC RD, OFFSET <=> RD = PC + (OFFSET << 12)
         */
        AUIPC,

        // JUMP
        // L 指 LINK（把下一条指令的地址加载到寄存器里用做返回地址），R 指寄存器跳转
        // JAL,
        // JALR,
        J,
        // JR,

        // 函数调用/返回
        CALL,
        TAIL,
        RET,

        // LOAD & STORE
        // LA 和 LLA 都是加载符号地址，LLA RD, SYMBOL <=> RD = &SYMBOL
        // 特别地 当编译位置无关代码时，LA 会去加载 GOT（全局偏移表中的地址），而
        // LLA 始终如上（LLA 的第二个 L 指 LOCAL）。 （PIC）LA RD, SYMBOL <=> RD =
        // GOT[PC + SYMBOL_GOT_OFFSET] (NON-PIC) 非 PIC 时 LA 和 LLA 一样。
        LA,
        // LLA,

        // 加载，没有 U 则符号扩展，32/64 位没有 LWU/LDU 因为 LW/LD
        // 在对应位架构直接一整个全加载了
        LW,
        SW,

        // 加载立即数
        // 对 LI，汇编器会根据 立即数的大小 自动生成对应加载指令
        LI,
        LUI,

        // SYSTEM CALL
        // ECALL,   // 环境调用异常
        // EBREAK,  // 调试器断点异常

        END_RV64IM,

        BEGIN_RV64FD,
        // FLOAT
        FNEG_S,
        FADD_S,
        FSUB_S,
        FMUL_S,
        FDIV_S,

        // 浮点转换 S 单精度 D 双精度 W 字 L 64 位整数（指 LP64 LONG）
        FCVT_S_W,
        FCVT_W_S,

        // LOAD & STORE
        FLW,
        FSW,

        // CMP
        FEQ_S,
        FLE_S,
        FLT_S,

        // FLOAT MV， X 指整数寄存器
        FMV_S,
        FMV_W_X,
        FMV_X_W,

        END_RV64FD,
    };


// i 指 整数寄存器
// f 值 浮点数寄存器
// 12-bits im/20-bits im 指 12/20 位长的（有符号/无符号）立即数
// symbol 指汇编中类似 .main（函数符号） .L1（跳转地址符号），可能有替换的需求（变量）

    enum rv_inst_cate {
        One,
        I,
        R,
        B,
        OneF,
        RF,
        Other,
    };

    /**
     * @brief 用来鉴别指令归类的变量模板
     */
    template <RiscVInst inst>
    inline constexpr rv_inst_cate RiscVInstCate = rv_inst_cate::Other;

    /**
     * @brief 用于存储指令类型信息的类型
     */
    template <RiscVInstRt RT, RiscVInstArg... Args>
    struct RiscVInstType {
        using return_type = RT;
        using args = std::tuple<Args...>;
        inline static constexpr auto arg_size = std::tuple_size<args>{}();
    };

    /**
     * @brief 用来记录指令指令对应类型的模板(的主模版)
     */
    template <RiscVInst rv_inst>
    struct _rv_inst_type {using type = void;};

    /**
     * @brief 用于 **访问** 指令的类型信息的别名模板
     */
    template <RiscVInst rv_inst>
    using RV_Type = typename _rv_inst_type<rv_inst>::type;

    namespace {
        template <typename RT, RiscVInst inst, typename... args>
        constexpr RiscVInst _details_select_inst = inst;
        template <RiscVInstRt RT, RiscVInst inst, RiscVInstArg... args>
        using _details_select_rv_type = RiscVInstType<RT, args...>;
    }

#pragma push_macro("DECL")
#pragma push_macro("RV_CATE")
#pragma push_macro("DECL1")
#pragma push_macro("DECLR")
#pragma push_macro("DECLI")
#pragma push_macro("DECLB")
#pragma push_macro("DECL1F")
#pragma push_macro("DECLRF")

#define DECL(...) template<> \
struct _rv_inst_type<_details_select_inst<__VA_ARGS__>> { \
    using type = _details_select_rv_type<__VA_ARGS__>;                             \
};

#define RV_CATE(name, cate) template<> \
    inline constexpr rv_inst_cate RiscVInstCate<name> = cate; \
// i32

// i -> i
#define DECL1(name) DECL(i_reg, name, i_reg) RV_CATE(name, rv_inst_cate::One)
    DECL1(NEG)
    DECL1(NEGW)
    DECL1(NOT)

// (i, i) -> i
#define DECLR(name) DECL(i_reg, name, i_reg, i_reg) RV_CATE(name, rv_inst_cate::R)
    DECLR(ADD)
    DECLR(SUB)
    DECLR(MUL)
    DECLR(DIV)
    DECLR(REM)

    DECLR(AND)
    DECLR(XOR)
    DECLR(OR)

    DECLR(SLL)
    DECLR(SRL)
    DECLR(SRA)

    DECLR(SLT)

// (i, 12bits signed imm) -> i
#define DECLI(name) DECL(i_reg, name, i_reg, i_bits_im<12>) RV_CATE(name, rv_inst_cate::I)
    DECLI(ADDI)

    DECLI(ANDI)
    DECLI(XORI)
    DECLI(ORI)

    DECLI(SLLI)
    DECLI(SRLI)
    DECLI(SRAI)

    DECLI(SLTI)

// (i, symbol) -> void
#define DECLB(name) DECL(void, name, i_reg, symbol_arg) RV_CATE(name, rv_inst_cate::B)
    DECLB(BEQ)
    DECLB(BNE)

// 不定型的指令

    DECL(i_reg, AUIPC, i_bits_im<20>)

    DECL(void, J, symbol_arg)

    DECL(void, CALL, symbol_arg)
    DECL(void, TAIL, symbol_arg)
    DECL(void, RET)


    DECL(i_reg, LA, symbol_arg)

// lw rd, offset(rs)
    DECL(i_reg, LW, i_bits_im<12>, i_reg)
// sw rs2, offset(rs1)
    DECL(void, SW, i_reg, i_bits_im<12>, i_reg)

    DECL(i_reg, LI, i_bits_im<12>)
    DECL(i_reg, LUI, i_bits_im<20>)

// float
// f -> f
#define DECL1F(name) DECL(f_reg, name, f_reg) RV_CATE(name, rv_inst_cate::OneF)
    DECL1F(FNEG_S)
// (f, f) -> f
#define DECLRF(name) DECL(f_reg, name, f_reg, f_reg) RV_CATE(name, rv_inst_cate::RF)
    DECLRF(FADD_S)
    DECLRF(FSUB_S)
    DECLRF(FMUL_S)
    DECLRF(FDIV_S)

// 浮点转换 S 单精度 D 双精度 W 字 L 64 位整数（指 LP64 LONG）
    DECL(i_reg, FCVT_S_W, f_reg)
    DECL(f_reg, FCVT_W_S, i_reg)

// LOAD & STORE
    DECL(f_reg, FLW, i_bits_im<12>, i_reg)
    DECL(void, FSW, f_reg, i_bits_im<12>, i_reg)

// CMP
    DECL(i_reg, FEQ_S, f_reg, f_reg)
    DECL(i_reg, FLE_S, f_reg, f_reg)
    DECL(i_reg, FLT_S, f_reg, f_reg)

// FLOAT MV， X 指整数寄存器
    DECL(f_reg, FMV_S, f_reg)
    DECL(i_reg, FMV_W_X, f_reg)
    DECL(f_reg, FMV_X_W, i_reg)

// END_RV64FD,

#pragma pop_macro("DECL")
#pragma pop_macro("RV_CATE")
#pragma pop_macro("DECL1")
#pragma pop_macro("DECLR")
#pragma pop_macro("DECLI")
#pragma pop_macro("DECLB")
#pragma pop_macro("DECL1F")
#pragma pop_macro("DECLRF")


}

#endif //SYSYUST_RISCV_INST_H
