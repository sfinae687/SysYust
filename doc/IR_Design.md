---
title: IR 设计（draft）

---

# IR 设计（draft）

参考 [llvm-ir](https://llvm.org/docs/MIRLangRef.html), [mlir](https://mlir.llvm.org/docs/LangRef/), [koppa-ir](https://github.com/pku-minic/koopa), 往年比赛队伍的项目。

使用 SSA-CFG with Basic Block Argument

## 编译流程

AST *(antlr4 ouput*)*
-> *(IR#1)*  $AST_1$ （AST rewriting 一些结构上的重写，展开 condexpr -> if 语句，常量求值）
-> *(IR#2)*  内存形式（load / store）的 IR 
-> *(IR#2？)*  非内存形式的 IR [next stage] 
<!--
-> $ir_1$ -> .. [next stage]
-->
-> CodeGen

## 基本块

- 基本形式是 控制流图CFG，每个节点是一个基本块。
- 基本块形如（只是书写格式，实现用数据结构形式）

  ```llvm
  .<name>(<r0>, <r1>): {
      <instructions>
      branch rcond, .<iftrue>(...), .<iffalse>(...) / goto .<next>(...)
  }
  ```

  ```bnf
  Branch ::= "br" Value "," SYMBOL [BlockArgList] "," SYMBOL [BlockArgList];
  Goto ::= "goto" SYMBOL [BlockArgList];
  BlockArgList ::= "(" Value {"," Value} ")";

  Block ::= "." SYMBOL [BlockParamList] ":" "{" {Statement} EndStatement "}";
  BlockParamList ::= "(" SYMBOL {"," SYMBOL} ")";
  ```

- Basic Block Argument 相当于 phi 的变体[[1]](https://en.wikipedia.org/wiki/Static_single-assignment_form#Block_arguments)[[2]](https://blog.maxxsoft.net/index.php/archives/143/)

## 指令（Instruction）

- 可扩展
- 接口可能易变

1. `alloc` / `load` / `store` ([maybe] 可以不用)
   - `r0 = alloc <type>` 为 r0 分配类型大小的空间，r0 类型为 `ptr Type`
   - `store <val>, r0` 为 `*r0 = <val>`
   - `r1 = load r0` 为 `r1 = *r0`
2. `branch` / `goto` (/ `switch` 用于优化时合并多分支跳转 <del>我要思维驰😋</del> [next stage])
   - `branch r0, .bbtrue(r1, ...), .bbfalse(r2, ...)` `r0 == 1` 时跳转 `.bbtrue`，否则跳转 `.bbfalse`
   - `goto .bb(r1, ...)`
3. `call` / `return`
   - `call func(r0, r1, r2...)` 调用函数
4. `GetElementPtr` 用来计算数组元素指针（一种安全的指针算数，用于指针分析，目前没有引入 `ptr2int` 和 `int2ptr`）
   - r0 为 `[4 x [5 x i32]]]` （`ptr` 看成数组）
     `r1 = getelementptr r0, <ind1>, <ind2>...` 即 `r1 = &(r0[ind1][ind2][...])`
     多参数形式目前好像是优化过程中有用（未确定）？再👀👀。[draft]
   - 或者改为单参形式 [optional]
     - r0 为 `[4 x [5 x i32]]]`
       `r1 = getelementptr r0, <ind1>` 即 `r1 = &(r0[ind1])`
5. `unreachable`
   - 不可达（也许可以不用这个）[maybe]
6. Unary/Binary Operator（似乎不用支持 `i64`，可能在后端指令选择时有优化，或者看之后优化）
   - `i32` `(-)` `+` `-` `*` `/` `%` `tofloat` `<=>`
     - `neg` // 后续可能去掉 [next stage]
     - `add`
     - `sub`
     - `mul` (无符号 `umul` `udiv` 和 `urem` 不用支持)
     - `div`
     - `rem`
     - `i2float`
     - `cmp`
       - `cmp ==`
       - `cmp >`
       - ...
   - `float`` (-)` `+` `-` `*` `/` `toint` `<=>`
     - `fneg`
     - `fadd`
     - `fsub`
     - `fmul`
     - `fdiv`
     - `float2i`
     - `fcmp`
   - `bool` （当成 `if` 语句，`!a` 当作 `a != 0`）
7. 全局变量的内存分配[todo]

## 可能加入[next stage]

1. `Atomic` / `Fence` 指令
2. `Vector` / `Struct` 类型
3. `switch` / `select` / `indirectbr` 更多分支指令
4. MLIR 方言类似指令（比如 循环多面体优化）
5. `tail call func(...)` 尾调用等指令属性
6. 其它类型
7. ...

## 类型（Type）

- 可扩展

1. `i32` （以及 `i64`）[maybe]
2. `float`
3. `Array` （数组，书写格式如：`[3 x [5 x i32]]`)
   - 有初始化器的值（特别地，有零初始化器）
4. `ptr` (64 位指针，带有所指类型信息)

## IR 结构

- `IR`
  - `Vec<GlobalVariable>` 全局变量列表
  - `Vec<Function>` 函数列表（带有函数声明或者两个分开[draft]）
  - `SymbolTable` 符号表
- `Function`
  - `Vec<BasicBlock>` 基本块表
  - `Vec<Argument>` 形参表
  - `Vec<Attr>` 属性
  - `CFG` 指针
  - `SymbolTable` 符号表
- `BasicBlock`
  - `ParentFunction`
  - `Vec<Instruction>`
  - `Terminator` (`br` / `goto` / `ret`)
- `Insturction`
  - `call`
  - `getelemptr`
  - `unreachable`
  - `arith operator`
- `CFG`

[todo]

## IR 遍历

MLIR 的数据流图是由 Operation 和 Value 构成的。MLIR 官网上，IR Structure 里面的 [两幅图](https://mlir.llvm.org/docs/Tutorials/UnderstandingTheIRStructure/#traversing-the-def-use-chains) 将 MLIR 的图结构解释得非常清楚：

首先是 **Operation 的连接**：

* Value 要么来自于 Operation 的 Result 要么来自于 BlockArgument
* 每个 Operation 的 Operand 都是到 Value 的指针
* 要修改 Operand 的时候，实际修改的应该是 OpOperand

(其中 `Value` 是 SSA Value)
### Data Flow
![](https://mlir.llvm.org/includes/img/DefUseChains.svg)

然后，是 **Value 的 use-chain**：

* 每个 Value 都将其 User 连接在一起了

### Def-Use Chain
![](https://mlir.llvm.org/includes/img/Use-list.svg)

可见，MLIR 的图是一个双向的图结构，在遍历尤其是修改的时候需要特别小心。

* 在修改 OpOpeand 的时候，对应 value 的 use-chain 会暗中被 MLIR 改掉
* 在调用 `value->getDefiningOp()` 的时候，BlockArgument 会返回 null



###  5.2. <a name='mlir-数据流图的遍历与修改'></a>MLIR 数据流图的遍历与修改

MLIR 数据流图的遍历往往遵循一种模式：Operation 调用函数找 Value，再用 Value 调用函数找 Operation，交替进行。

其中，**Operation 找 Value 的方法**有：

* **getOperands**、**getResults**：这两个非常常用，如下面的代码可以用来 Op 找 Op
    ```cpp
    for(auto operand: op->getOperands()) {
      if(auto def = operand.getDefiningOp()) {
        // do something
      }
      else {
        // block argument
      }
    }
    ```
* **getOpOperands**：这个在需要更改 operands 的时候非常有用，例如下面的代码将 value 做替换：
    ```cpp
    IRMapping mapping;
    // 将 op1 的 results 映射到 op2 的 results
    mapping.map(op1->getResults(), op2->getResults());
    for(auto &opOperand: op3->getOpOperands()) {
      // 将 op3 的参数里含有 op1 results 的替换为 op2 的
      // lookupOrDefault 指找不到 mapping 就用原来的
      opOperand.set(mapping.lookupOrDefault(opOperand.get()));
    }
    ```

**Value 找 Op 的方法**有：

* **getDefiningOp**：可能返回 null
* **getUses**：返回 OpOperand 的迭代器
* **getUsers**：返回 Operation 的迭代器

**Op的getUses和getUser**：operation 也有 getUses 和 getUsers 函数，等价于把这个 op 的所有 result 的 Uses 或 Users 拼在一起。

**Value的修改**：Value 支持 **replaceAllUseWith** 修改，一种*看起来*等价的代码是：
```cpp
for(auto & uses: value.getUses()) {
  uses.set(new_value);
}
```
但需要注意，上面的代码是**非常危险**的。因为在 uses.set 的时候，会修改 value 的 use chain，而 value 的 use-chain 正在被遍历，可能一修改就挂了。于是，最好用 mlir 提供好的 `replaceAllUseWith` 来修改。

###  <a name='mlir-控制流图的遍历与修改'></a>MLIR 控制流图的遍历与修改

与数据流图相比，控制流图遍历更简单，常用的一些函数：

* **op.getParentOp**, **op.getParentOfType**：获取父亲Op
* **op.getBlock**：注意是返回父亲block，而不是函数block
* **op.getBody**：这个才是返回内部 block / region

遍历儿子的方法：

* **op.walk**：递归地遍历所有子孙op：

    ```cpp
    // 递归遍历所有儿子
    func.walk([](Operation * child) {
      // do something
    });
    // 递归遍历所有是 `ReturnOp` 类型的儿子
    func.walk([](ReturnOp ret) {
      // do something
    })
    ```
    
* **block**：直接就是一个 iterator，可以直接遍历：

    ```cpp
    Block * block = xxx
    for(auto & item: *block) {
      // do something
    }
    ```

其他遍历方法如 `getOps<xxx>` 可以自行尝试。

控制流图的修改主要用 `OpBuilder` 完成。强烈建议把找到 `OpBuilder` 的代码，把里面有什么函数都看一看，常见的：

* **builder.create**：创建op
* **builder.insert**：插入remove的op
* **op->remove()**：从当前块移除，但不删除，可以插入到其他块内
* **op->erase()**：从当前块移除，并且删除

**删除顺序**：在删除一个 op 的时候，这个 op 不能存在 user，否则会报错。


## 示例

快速幂：

```c
int qpow(int x, int y) {
    int r = 1;
    while (y != 0) {
        if (y / 2 * 2 != y) { // y 是奇数 好像没有 bitwise and 🤔
            r = r * x;
        }
        
        x = x * x;
        y = y / 2;
    }
    
    return r;
}
```

load / store 形式的 IR

```llvm
i32 qpow(x_param, y_param) {
    .bb_head: { ; 不使用寄存器
        x = alloc i32
        y = alloc i32
        store x_param, x
        store y_param, y
    }

    .bb0: { ; int r = 1;
        r = alloc i32
        store 1, r
    }
    
    .bb1: { ; while condition
        t1 = (y != 0) ; icmp ne y, 0
        
        br t1, .bb2, .bb4
    }
    
    .bb2: { ; if
        t2 = load y
        t3 = t2 / 2
        t4 = t3 * 2
        t5 = (t4 == t2)
        
        br t5, .bb3, .bb4
    }
    
    .bb3: { ; iftrue
        t6 = load r
        t7 = load x
        t8 = t6 * t7 ; r = r * x
        store t8, r
        
        goto .bb2
    }
    
    .bb4: {
        t9 = load x
        t10 = load x
        t11 = t9 * t10 ; x = x * x
        store t11, x
        
        t12 = load y
        t13 = t12 / 2 ; y = y / 2
        
        goto .bb1 ; 回到循环条件
    }
    
    .bb5: { ; return
        t14 = load r
        return t14
    }
}

```

basic block argument 的 IR

```llvm
i32 qpow(x_param, y_param) {
    .bb0: { ; int r = 1;
        goto .bb1(1, x_param, y_param)
    }
    
    .bb1(r, x, y): { ; while condition
        t1 = (y != 0) ; icmp ne y, 0
        
        br t1, .bb2, .bb4
    }
    
    .bb2: { ; if
        t2 = y / 2
        t3 = t2 * 2
        t4 = (t2 != t3)
        
        br t5, .bb3, .bb4(r)
    }
    
    .bb3: { ; iftrue
        t6 = load r
        t7 = load x
        r1 = t6 * t7 ; r = r * x
        
        goto .bb2
    }
    
    .bb4(r2): { ; r2 = phi r r1
        x2 = x * x ; x = x * x
        
        y2 = y / 2
        
        goto .bb1(r2, x2, y2) ; 回到循环条件
    }
    
    .bb5: { // return
        retur;r
    }
}
```

llvm ir

```llvm
define dso_local signext i32 @qpow(i32 noundef signext %0, i32 noundef signext %1) #0 !dbg !12 {
  br label %3, !dbg !22

3:                                                ; preds = %14, %2
  %4(x) = phi i32 [ %0, %2 ], [ %x1, %14 ]
  %5(y) = phi i32 [ %1, %2 ], [ %9, %14 ]
  %6(r) = phi i32 [ 1, %2 ], [ %r2, %14 ]
  
  %7 = %y != 0
  br i1 %7, label %8, label %17

8:                                                ; preds = %3
  %9 = %y / 2
  %10 = %9 * 2
  %11 = %10 != %y
  br i1 %11, label %12, label %14

12:                                               ; preds = %8
  %r1 = %r * %x
  
  br label %14

14:                                               ; preds = %12, %8
  %r2 = phi i32 [ %r1, %12 ], [ %r, %8 ]
  
  %x1 = %x * %x
  
  br label %3
17:                                               ; preds = %3
  ret i32 %6
}

```
