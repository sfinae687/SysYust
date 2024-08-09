---
title: 代码生成的语义（draft）

---

# 代码生成的语义（draft）

- lowering IR to ASM

中端 IR 
-> 指令选择
-> MIR
-> 消除基本块参数
-> 寄存器分配
-> 前后言插入
-> 基本块拓扑排序
-> 直接输出 MIR 到 Risc-v 汇编

- MIR
  是 IR 的方言，只有指令不同
- 仅将 Inst 变成 MInst
- 寄存器分配后的 var 不是 SSA 并且为寄存器



## Risc-V 的函数调用约定

[具体规范](https://github.com/riscv-non-isa/riscv-elf-psabi-doc)
[一个说明](https://pku-minic.github.io/online-doc/#/lv8-func-n-global/func-def-n-call)

## 寄存器 ABI
RISC-V 的 [ABI](https://en.wikipedia.org/wiki/Application_binary_interface) 中, 32 个整数寄存器的名称和含义如下表所示:

| 寄存器     | ABI 名称   | 描述               | 保存者    |
| -         | -         | -                 | -         |
| `x0`      | `zero`    | 恒为 0             | N/A       |
| `x1`      | `ra`      | 返回地址            | 调用者    |
| `x2`      | `sp`      | 栈指针             | 被调用者   |
| `x3`      | `gp`      | 全局指针            | N/A      |
| `x4`      | `tp`      | 线程指针            | N/A      |
| `x5`      | `t0`      | 临时/备用链接寄存器  | 调用者     |
| `x6-7`    | `t1-2`    | 临时寄存器          | 调用者    |
| `x8`      | `s0`/`fp` | 保存寄存器/帧指针    | 被调用者   |
| `x9`      | `s1`      | 保存寄存器          | 被调用者   |
| `x10-11`  | `a0-1`    | 函数参数/返回值      | 调用者    |
| `x12-17`  | `a2-7`    | 函数参数            | 调用者    |
| `x18-27`  | `s2-11`   | 保存寄存器          | 被调用者   |
| `x28-31`  | `t3-6`    | 临时寄存器          | 调用者    |
| `f0-7`    | `ft0-7`   | 浮点临时寄存器       | 调用者    |
| `f8-9`    | `fs0-1`   | 浮点保存寄存器       | 被调用者  |
| `f10-f11` | `fa0-1`   | 浮点参数/返回值      | 调用者    |
| `f12-17`  | `fa2-7`   | 浮点参数            | 调用者    |
| `f18-27`  | `fs2-11`  | 浮点保存寄存器       | 被调用者  |
| `f28-31`  | `ft8-11`  | 浮点临时寄存器       | 调用者    |

## 前言和尾言（Prologue/Epilogue）生成（摘自 koppa ir)

![call-with-10-args](https://hackmd.io/_uploads/ByqFIJtvC.png)

1. 扫描 Koopa IR 程序中的每一个函数, 对所有的函数, 都像生成 `main` 一样生成代码.
2. 生成 prologue 之前, 扫描函数内的每条指令, 统计需要为局部变量分配的栈空间 $S$, 需要为 `ra` 分配的栈空间 $R$, 以及需要为传参预留的栈空间 $A$, 上述三个量的单位均为字节.
    * $R$ 的计算方法: 如果函数中出现了 `call`, 则为 4, 否则为 0.
    * $A$ 的计算方法: 设函数内有 $n$ 条 `call` 指令, $call_i$ 用到的参数个数为 $len_i$, 则 $A = \max \{ \max_{i=0}^{n-1} len_i - 8, 0 \} \times 4$
3. 计算 $S + R + A$, 向上取整到 16, 得到 $S^\prime$.
4. 生成 prologue: 首先, 根据 $S^\prime$ 生成 `addi` 指令来更新 `sp`. 然后, 如果 $R$ 不为 0, 在 $sp + S^\prime - 4$ 对应的栈内存中保存 `ra` 寄存器的值.
5. 生成函数体中的指令.
6. 如果遇到 Koopa IR 中的 `call` 指令, 你需要先将其所有参数变量的值读出, 存放到参数寄存器或栈帧中, 然后再生成 RISC-V 的 `call`.
7. 生成 epilogue: 如必要, 从栈帧中恢复 `ra` 寄存器. 然后, 复原 `sp` 寄存器的值. 最后生成 `ret`.

## IR

1. `alloc` / `load` / `store`
   - `r0 = alloc <type>` 不生成，新建并维护当前变量在栈中的偏移量。
   - `store <val>, r0` -> `sd val , {stack_offset(r0)} (sp)`
   - `r1 = load r0` -> `ld r1, {stack_offset(r0)} (sp)`
2. `branch` / `goto`
   - basic block argument
     - 在 第一个 stage 不会出现，在加入后有一个 Pass 会消除它[llvm-elim-phi](https://llvm.org/doxygen/PHIElimination_8cpp_source.html)
   - `branch r0, .bbtrue, .bbfalse` `r0 == 1` 时跳转 `.bbtrue`，否则跳转 `.bbfalse`
     ```
     bnz r0, .bbtrue
     j .bbfalse
     ```
   - `goto .bb`
     - `j .<label>`
3. `call` / `return`
   - `call func(r0, r1, r2...)` 调用和返回函数，见函数调用前言和尾言（Prologue/Epilogue）生成。
4. `GetElementPtr` 用来计算数组元素指针（一种安全的指针算数，目前没有引入 `ptr2int` 和 `int2ptr`）
   - r0 为 `[4 x [5 x i32]]]` （`ptr` 看成数组）
     `r1 = getelementptr r0, <ind1>, <ind2>...` 即 `r1 = &(r0[ind1][ind2][...])`
     生成时转换为单参形式[draft]
   - 单参形式 [optional]
     - r0 为 `[4 x [5 x i32]]]`
       `r1 = getelementptr r0, <ind1>` 即 `r1 = &(r0[ind1])`
       -> `r1 = r0 + size(type) * ind1` 再生成
       
5. `unreachable`
   - 不可达（也许可以不用这个）[maybe]
   - 优化中应该消失。
6. Unary/Binary Operator
   常量表达式应在优化中消失，在这里抛出 Warning
   当所有参数全是立即数时 -> `li rd, <result>`，并发出 Warning
   `rimm` 表示要立即数所加载的寄存器，特别地，当立即数为 0 时，`rimm = x0`
   - `i32` `(-)` `+` `-` `*` `/` `%` `tofloat` `<=>`
     - `neg` -> `negw` 
     - `r2 = add r0, r1` 
        - 有一个立即数 -> `addiw r2, reg, imm`
        - 全是寄存器 -> `addw r2, r0, r1`
     - `r2 = sub r0, r1`
        - 有一个立即数 -> `addiw r2, reg, -imm`
        - 全是寄存器 -> `subw r2, r0, r1`
     - `r2 = mul r0, r1` 
        - 有一个立即数 -> `mulw r2, reg, rimm`
        - 全是寄存器 -> `mulw r2, r0, r1`
     - `r2 = div/rem r0, r1``
       - r1 是立即数 -> 使用转换算法[todo]
       - r0 是立即数/都是寄存器 -> `divw/remw r2, rimm0/r0, r1`
     - `r1 = i2float r0` -> `fcvt.s.w r1, r0`
     - `r2 = cmp op r1, r0`
       - 有立即数 -> 使用转换算法[todo]
       - 无立即数 -> `bop r2, r1, r0` 
       - `==` -> `xor`
       - '!=' ->` xor, snez`
       - `>`  -> `(rev)slt` （rev 表示参数位置反转）
       - '>=' -> `(rev)slt, xori 1`
       - '<=' -> `slt, xori 1`
       - '<'  -> `slt`
   - `float` `(-)` `+` `-` `*` `/` `toint` `<=>`
     - `fneg` -> `fneg.s`
     - `fadd` -> `fadd.s rimm/r0`
     - `fsub` -> `fsub.s rimm/r0`
     - `fmul` -> `fmul.s rimm/r0`
     - `fdiv` 
        - 有立即数 -> 使用转换算法[todo]
        - 无立即数 -> fdiv.s 
     - `float2i` -> `fcvt.s.w`
     - `fcmp`
       有立即数 -> 使用转换算法<del>奇奇怪怪的优化</del>[todo]
       无立即数 -> `bop r2, r1, r0` 
       - `==` -> `feq.s`
       - '!=' -> `feq.s, xori`
       - `>`  -> `(rev)flt.s`
       - '>=' -> `(rev)fle.s`
       - '<=' -> `fle.s`
       - '<'  -> `flt.s`
   - `bool` （当成 `if` 语句，`!a` 当作 `a != 0`）
     - 在 AST rewriting 中？
7. 全局变量的内存分配
   - i32 / float 
     ```asm
     <symbol>:
       .data <value>
     ```
   - array
     按内存布局线性生成 `.data <val>`
     遇到零初始化器生成 `.zero <num> * size(<type>)`
     例： `int a[1000][10] = {{1, 3}, {2, 3}};`
     ```asm
     a:
        .word   1                               # 0x1
        .word   3                               # 0x3
        .zero   32
        .word   2                               # 0x2
        .word   3                               # 0x3
        .zero   32
        .zero   39920
     ```
     
## 指令结构体
全部指令参见 https://hackmd.io/Vlt97GIiRDylJFEiY9YE-A
```cpp
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
    // 用以获得乘法运算溢出的高位
    MULH,
    MULHSU,
    MULHU,
    DIV,
    DIVU,
    REM,
    REMU,

    // 32:
    NEGW,
    ADDIW,
    ADDW,
    SUBW,
    /**
     * 特别地，RISC-V 64 的数（有符号无符号）都以 64 位存储，只在运算过程中截断
     * MULW A, B, C <=> A = SEXT64(TRUNC32(B * C))
     * MUL A, B, C <=> A = TRUNC64(B * C)
     */
    MULW,
    DIVUW,
    DIVW,
    REMUW,
    REMW,
    SEXT_W,  // 符号扩展

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

    SLLIW,
    SLLW,
    SRLIW,
    SRLW,
    SRAIW,
    SRAW,

    // BRANCH
    BEQ,
    BEQZ,
    BGE,
    BGEU,
    BGEZ,
    BGT,
    BGTU,
    BGTZ,
    BLE,
    BLEU,
    BLEZ,
    BLT,
    BLTZ,
    BLTU,
    BNE,
    BNEZ,

    // CMP
    // 无符号比较跟有符号一样
    // 没有 SEQ, 它等价于 XOR A, A, B; SEQZ DST, A
    SLT,
    SLTI,
    SLTIU,
    SLTU,
    SLTZ,
    SNEZ,
    SGTZ,
    SEQZ,

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
    JAL,
    JALR,
    J,
    JR,

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
    LLA,

    // 加载，没有 U 则符号扩展，32/64 位没有 LWU/LDU 因为 LW/LD
    // 在对应位架构直接一整个全加载了
    LB,  // 字节
    LBU,
    LH,  // 半字（2 字节 16 位）
    LHU,
    LW,  // 字（4 字节 32 位）
    LWU,
    LD,  // 双字（8 字节 64 位）

    // 存就不用区分符号扩展了
    SB,  // 呜呜呜
    SH,
    SW,
    SD,

    // 加载立即数
    // 对 LI，汇编器会根据 立即数的大小 自动生成对应加载指令
    LI,
    LUI,

    // SYSTEM CALL
    ECALL,   // 环境调用异常
    EBREAK,  // 调试器断点异常

    END_RV64IM,

    BEGIN_RV64FD,
    // FLOAT
    FNEG_D,
    FNEG_S,
    FABS_D,
    FABS_S,
    FADD_D,
    FADD_S,
    FSUB_D,
    FSUB_S,
    FMUL_D,
    FMUL_S,
    FDIV_D,
    FDIV_S,
    FSQRT_D,  // 惊喜！开平方
    FSQRT_S,

    // 三路浮点 D = A * B +/- C
    // 加 N 代表 D = -(A * B) +/- C
    FMADD_D,
    FMADD_S,
    FMSUB_D,
    FMSUB_S,
    FNMADD_D,
    FNMADD_S,
    FNMSUB_D,
    FNMSUB_S,

    // 浮点类型
    /**
     * DST位 含义
     * 0     SRC为−∞。
     * 1     SRC是负规格化数。
     * 2     SRC是负的非规格化数。
     * 3     SRC是-0。
     * 4     SRC是+0。
     * 5     SRC是正的非规格化数。
     * 6     SRC是正的规格化数。
     * 7     SRC为+∞。
     * 8     SRC是信号(SIGNALING)NAN。
     * 9     SRC是一个安静(QUIET)NAN。
     */
    FCLASS_D,
    FCLASS_S,

    // 浮点转换 S 单精度 D 双精度 W 字 L 64 位整数（指 LP64 LONG）
    FCVT_S_D,
    FCVT_S_L,
    FCVT_S_LU,
    FCVT_S_W,
    FCVT_D_L,
    FCVT_D_LU,
    FCVT_D_S,
    FCVT_D_W,
    FCVT_D_WU,
    FCVT_L_D,
    FCVT_L_S,
    FCVT_LU_D,
    FCVT_LU_S,
    FCVT_W_D,
    FCVT_WU_D,
    FCVT_W_S,
    FCVT_WU_S,

    // LOAD & STORE
    FLW,
    FSW,
    FLD,
    FSD,

    // CMP
    FEQ_D,
    FEQ_S,
    FLE_D,
    FLE_S,
    FLT_D,
    FLT_S,

    FMAX_D,
    FMAX_S,
    FMIN_D,
    FMIN_S,

    // FLOAT MV， X 指整数寄存器
    FMV_S,
    FMV_D,
    FMV_D_X,
    FMV_W_X,
    FMV_X_D,
    FMV_X_W,

    // 浮点注入
    // 第一个操作数的值 + 第二个操作数的符号 + 符号 N 取反，X 异或
    FSGNJ_D,
    FSGNJ_S,
    FSGNJN_D,
    FSGNJN_S,
    FSGNJX_D,
    FSGNJX_S,

    END_RV64FD,
};
```