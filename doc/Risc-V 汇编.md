---
title: Risc-V 汇编

---

# Risc-V 汇编

[参考资料](https://hackmd.io/fAXm5yBvQh-NUKazDEJbkQ)
[非特权标准](https://riscv.org/wp-content/uploads/2019/12/riscv-spec-20191213.pdf)
[一个操作系统课的教程](https://gitee.com/hustos/pke-doc/blob/master/chapter1_riscv.md)

## 指令列表（方便写 enum）

指令结构体（裁剪后）见 https://hackmd.io/kid1kJIiT4KrrGfXLbPz1A#%E6%8C%87%E4%BB%A4%E7%BB%93%E6%9E%84%E4%BD%93

`*W` 指令只是截取操作数后 32 位

```cpp
BEGIN_RV64IM
// 后面加 p 表示伪指令
mv p
nop p

// 64:
neg p
add
addi
sub
mul 
// 用以获得乘法运算溢出的高位
mulh
mulhsu
mulhu
div
divu
rem
remu

// 32:
negw p
addiw
addw
subw
/**
* 特别地，risc-v 64 的数（有符号无符号）都以 64 位存储，只在运算过程中截断
* mulw a, b, c <=> a = sext64(trunc32(b * c))
* mul a, b, c <=> a = trunc64(b * c)
*/
mulw 
divuw
divw
remuw
remw
sext.w p // 符号扩展

// bitwise 按位逻辑运算
and
andi
xor
xori
not p
or 
ori

// shift 移位指令
// s{l/r}{l/a} 第一个 {l/r} 表示左右，第个 {l/a} （logical/arithmetic）表示逻辑和算数（算数保持符号）
// 没有 sla 是因为它和 sll 是等价的
sll
slli
srl
srli
sra
srai

slliw
sllw
srliw
srlw
sraiw
sraw

// branch
beq
beqz p
bge
bgeu
bgez p
bgt p
bgtu p
bgtz p
ble p
bleu p
blez p
blt
bltz p
bltu
bne
bnez p

// cmp 
// 无符号比较跟有符号一样
// 没有 seq, 它等价于 xor a, a, b; seqz dst, a 
slt
slti
sltiu
sltu
sltz p
snez p
sgtz p
seqz p

// 处理 pc 高位偏移
/**
* 在 Risc-V 里，由于指令长度的问题（64 位指令可能存有多个参数/立即数，指令中提供给立即数的长度有限），偏移有时无法完全加载（比如 63 位长的偏移），这时要分为高位和低位分开加载。
* 比如 jalr dst，offset(src) offset 只能是 12 位立即数
* 在加载地址类偏移时，理想的状况是 dst = pc + offset 得到偏移后的地址
* 加载高位和低位情况时 dst = pc + offsetHi + offsetLo (offset = offsetHi + offsetLo)
* 可以使用 dst1 = pc + offsetHi; jalr dst, offsetLo(dst1) 来解决
* 但是由于 Risc-V 的设计，你不能直接访问 pc
* 那么（pc + offsetHi）部分就产生了一个指令 auipc（add ui to pc）
* auipc rd, offset <=> rd = pc + (offset << 12)
*/
auipc

// jump
// l 指 link（把下一条指令的地址加载到寄存器里用做返回地址），r 指寄存器跳转
jal
jalr
j p 
jr p  

// 函数调用/返回
call p
tail p
ret p

// load & store
// la 和 lla 都是加载符号地址，lla rd, symbol <=> rd = &symbol
// 特别地 当编译位置无关代码时，la 会去加载 GOT（全局偏移表中的地址），而 lla 始终如上（lla 的第二个 l 指 local）。
// （PIC）la rd, symbol <=> rd = GOT[pc + symbol_got_offset]
// (non-PIC) 非 PIC 时 la 和 lla 一样。
la p
lla p

// 加载，没有 u 则符号扩展，32/64 位没有 lwu/ldu 因为 lw/ld 在对应位架构直接一整个全加载了
lb // 字节
lbu
lh // 半字（2 字节 16 位）
lhu
lw // 字（4 字节 32 位）
lwu
ld // 双字（8 字节 64 位）

// 存就不用区分符号扩展了
sb // 呜呜呜
sh
sw
sd

// 加载立即数
// 对 li，汇编器会根据 立即数的大小 自动生成对应加载指令
li p
lui

// 内存一致性指令
fence
fence.i

// system call
ecall // 环境调用异常
ebreak // 调试器断点异常

// CSR 控制状态寄存器指令 
csrr p // 读
csrw p // 写
csrwi p
csrc p // clear 按位与
csrci p
csrs p // set 按位或
csrsi p

csrrw // csrr + csrw 
csrrwi
csrrc // csrr + csrc
csrrci
csrrs // csrr + csrs
csrrsi

END_RV64IM

BEGIN_RV64FD
// float
fneg.d p
fneg.s p
fabs.d p
fabs.s p
fadd.d
fadd.s
fsub.d
fsub.s
fmul.d
fmul.s
fdiv.d
fdiv.s
fsqrt.d // 惊喜！开平方
fsqrt.s

// 三路浮点 d = a * b +/- c
// 加 n 代表 d = -(a * b) +/- c
fmadd.d
fmadd.s
fmsub.d
fmsub.s
fnmadd.d
fnmadd.s
fnmsub.d
fnmsub.s

// 浮点类型
/**
* dst位 含义
* 0     src为−∞。
* 1     src是负规格化数。
* 2     src是负的非规格化数。
* 3     src是-0。
* 4     src是+0。
* 5     src是正的非规格化数。
* 6     src是正的规格化数。
* 7     src为+∞。
* 8     src是信号(signaling)NaN。
* 9     src是一个安静(quiet)NaN。
*/
fclass.d
fclass.s

// 浮点转换 s 单精度 d 双精度 w 字 l 64 位整数（指 LP64 long） 
fcvt.s.d
fcvt.s.l
fcvt.s.lu
fcvt.s.w
fcvt.d.l
fcvt.d.lu
fcvt.d.s
fcvt.d.w
fcvt.d.wu
fcvt.l.d 
fcvt.l.s
fcvt.lu.d
fcvt.lu.s
fcvt.w.d
fcvt.wu.d
fcvt.w.s
fcvt.wu.s

// load & store
flw
fsw
fld
fsd

// cmp
feq.d
feq.s
fle.d
fle.s
flt.d
flt.s

fmax.d
fmax.s
fmin.d
fmin.s

// float mv， x 指整数寄存器
fmv.s p
fmv.d p
fmv.d.x
fmv.w.x
fmv.x.d
fmv.x.w

// 浮点 CSR
/** Rounding Mode (frm) | Accrued Exceptions (fflags) |
*                       |  NV  |  DZ  |  OF | UF | NX |
* frm 取整模式最近偶数、向零、向上、向下、向最近最大值
* NV 非法操作 DZ 除零 OF 上溢 UF 下溢 NX 不精确
*/
frcsr p // read fcsr
fscsr p // swap fcsr
frflags p
fsflags p
frrm p
fsrm p

// 浮点注入
// 第一个操作数的值 + 第二个操作数的符号 + 符号 n 取反，x 异或
fsgnj.d
fsgnj.s
fsgnjn.d
fsgnjn.s
fsgnjx.d
fsgnjx.s

END_RV64FD

BEGIN_RV64A
// atom 原子指令
amoadd.d amoadd.w amoand.d amoand.w
amomax.d amomax.w amomaxu.d amomaxu.w
amomin.d amomin.w amominu.d amominu.w
amoor.d amoor.w amoswap.d amoswap.w
amoxor.d amoxor.w 

// 加载保留/条件存入
lr.w
lr.d
sc.w
sc.d
END_RV64A

BEGIN_RV64C
// compress 压缩指令，这里没有写出
END_RV64C

// stat 统计周期数/已完成指令数/当前时间（程序性能计数器）
rdcycle p
rdcycleh p
rdinstret p
rdinstreth p
rdtime p
rdtimeh p

// privileged inst 特权指令
mret
sret
sfence.vma
wfi

```

CSR 布局表
| Number | Privilege | Name | Description |
| -- | -- | --- | --- |
| Floating-Point Control and Status Registers|
| 0x001 | Read/write | fflags | Floating-Point Accrued Exceptions. |
| 0x002 | Read/write | frm | Floating-Point Dynamic Rounding Mode. |
| 0x003 | Read/write | fcsr | Floating-Point Control and Status Register (frm + fflags). |
| Counters and Timers |
| 0xC00 | Read-only | cycle | Cycle counter for RDCYCLE instruction. |
| 0xC01 | Read-only | time | Timer for RDTIME instruction. |
| 0xC02 | Read-only | instret | Instructions-retired counter for RDINSTRET instruction. |
| 0xC80 | Read-only | cycleh | Upper 32 bits of cycle, RV32I only. |
| 0xC81 | Read-only | timeh | Upper 32 bits of time, RV32I only. |
| 0xC82 | Read-only | instreth | Upper 32 bits of instret, RV32I only. |