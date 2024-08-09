---
title: IR 生成的语义（draft）（文档完成度低）

---

# IR 生成的语义（draft）

AST -> IR lowering

## AST Pass

1. 常量求值
2. AST 重写
   - CondExpr
     转换成 `If` 语句
     
## CFG 生成

AST 的形式良好。
1. `If` 语句
   将 `If` 的两个 `Block` 分离成 BB
2. `While` 语句
   将 `While`
   ```c
   ---------
   |  pre  |
   ---------
       |
   --------- <-|
   |  cond |   |
   ---------   |
       |---------|
   ---------   | |
   |  body |   | |
   ---------   | |
       |-------- |
                 |
   --------- <---|
   |  next |
   ---------
   ```
   `break` 和 `continue` 分别直接跳转到 `cond` 和 `next` 处
3. return 语句
   截断并连接到 exit block
     
## 寄存器 load/store

目前似乎可以直接生成虚拟寄存器？
     
## 生成[todo]

对每个函数生成 IR

- `Stmt`
  - `Block`
     子节点:
     - `Stmt` 列表
       - 顺序生成
  - `If`
     - CFG
     子节点:
     - 表达式"条件"
     - 子句 `Stmt`
     - else子句 `Stmt`
  - `While`
    - CFG
    - 表达式"条件"
    - 子句 `Stmt`
  - `Break`
    - CFG
  - `Continue`
    - CFG
  - `Return`
      子节点:
      - 可空的 `Expr`.
  - `Assign`
      子节点:
      - 左值表达式且类型为 Number
      - 表达式 `Expr`
  - `Empty`
  - `Expr`
      描述信息:类型标识
      - `IntLiteral` 表达式类型 `Int`
         `li `
      - `FloatLiteral` 表达式类型为 `Float`
         ``
      - `Call`
         表达式类型为函数返回类型.
         描述信息:函数名标识  
         子节点:
         - `Expr` 列表
         `emit(r0, Expr)`
         `...`
         `call <func>(r0, r1, r2, ...)`
      - `UnaryOp` (`-`) 
         根据类型直接替换生成
         `-` -> `neg/fneg`
      - `BinaryOp` (`+` `-` `*` `/` `%`)
         根据类型直接替换生成
         `+` -> `add/fadd`
         `-` -> `sub/fsub`
         `*` -> `mul/fmul`
         `/` -> `div/fdiv`
         `%` -> `rem`
      - `LExpr` 左值表达式
          - `DeclRef` 变量的引用 表达类型为变量类型
          - `ArraySubscript` -> `GetElementPtr` 分离？
  - `CondExpr` 
     - 除 `Compare` 外皆转为 If 语句
     - `Compare` -> `cmp/fcmp` 

## SSA 生成

- SSA
  - [SSA book](pfalcon.github.io/ssabook/latest/)
  - [SSA book 2](https://link.springer.com/book/10.1007/978-3-030-80515-9)
  - [SSA 构造算法论文](https://link.springer.com/content/pdf/10.1007/978-3-642-37051-9_6.pdf)
  - [SSA 构造算法博客](https://roife.github.io/posts/braun2013/)

直接更改解释器

### 符号表
- ReadVar 读取变量并创建从 use 到 def 的基本块参数
  ```cpp
    if 在当前基本块 SymTable 中
        return SymTable[var];
    else findPredBB
    ```
- WriteVar 更新变量对应 SSAValue

### ReadVar
- findPredBB
    ```cpp
    if bb is not sealed
        bb.getArg.add(new blockargval)
    else if bb.pred.size() == 1
        val = ReadVar(var, bb.pred[0])
    else // 多个分支
        bb.getArg.add(new blockargval)
        for pred_bb in bb.pred
            pred_bb.terminate.thisblock.arg += ReadVar(var, pred_bb)
            TryRemoveTrivial(bb);
    WriteVar(var, bb, val)
    return val
    ```

### remove Trivial Phi
- TrivialityCheck(bb)
  ```cpp    
  if wit1 and wit2 uninit
      same = empty
      for op in phi
  ```

- 

## SSAValue

符号的实现方式：一种值语义的标记，在符号的字符部分长度固定的情况下可以进行 $O(1)$ 时间的哈希和比较。

变量符号的关联信息

- 标识量的类型 ：以内联存储在符号体的内部
- 全局变量标记 ：待决
- 参数变量标记 ：待决
- undef d


函数符号的关联信息

- 函数的返回类型 ：存储在符号表中
- 函数的副作用标记 ： 存储在符号表中