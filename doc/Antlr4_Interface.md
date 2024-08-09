---
title: Antlr4 接口速查

---

# Antlr4 接口速查

什么文档写的一坨的垃圾东西,不对它根本没写.😣

- [简化版文档](https://daichangya.github.io/antlr4-doc)
- [Java Runtime API](https://www.antlr.org/api/Java/)
- [Rust Runtime API](https://docs.rs/antlr-rust/latest/antlr_rust/)

## 运行时库摘要

### 类 ParserRuleContext

`ParaserRuleContext` 是 `ParserTree` 的派生类.  

#### 结构

该类是所有具体语法的语法元素上下文对象的积累。对于某一种语法元素比如 `[rule]` 它对应的上下文类是 `[rule]Context`. 这些 `ParserRuleContext` 子类都是该语法解析器类的成员类.  

它除了继承所有 `ParserRuleContext` 所有成员外,它还定义了一些对应它的语法构成的成员函数.如果 `[rule]` 的某一个生成式中包含了一个名为 `[subrule]` 的语法元素. 那么 `[rule]Context` 就会有一组名为 `[subrule]` 的成员函数用于获取该部分的上下文对象. 

根据生成式中 `[subrule]` 的数量决定 `[subrule]` 函数具有是什么. 如果仅有一个 `[subrule]` 那么 `[subrule]` 函数就会仅有一个无参数的版本,可以获取对应的上下文对象的指针. 如果存在一个生成式具有多个 `[subrule]` . 那么 `[subrule]` 函数的无参数版本就会返回上下文对象指针的 `std::vector`, 同时还会有一个带 `std::size_t` 参数的版本, 可以获取指定索引的上下文对象.

`ParserRuleContext` 类本身(以及它的父类的还定义了维护语法树结构的成员. 详细可见 [`ParserRuleContext` API 文档](https://www.antlr.org/api/Java/org/antlr/v4/runtime/ParserRuleContext.html).  

#### 信息

`ParserRuleContext` 除了获取完整文本的 `getText` 方法外,基本都是访问其树结构的各种方法.

可以通过终结符名称获取关于此节点的 `TerminalNode` 对象(这个类型也继承于 `ParserTree`), 通过该对象的 `getSymbol` 获取对应的 `Token` 对象. `Token` 对象可以获取这个终结符文本的 `getText` 和获取终结符类型的 `getType`.

### 流和解析器

`Parser` 类是解析器类. 对于具体的语法 `[Grammar]` , 它的目标的解析器类对应的类名是 `[Grammar]Parser` 类.  
若想要构造一个 `Parser` 对象,需要一个 `TokenStream` 对象作为构造器参数,一般使用 `CommonTokenStream` 类作为这个参数,  
构造这个类型的对象需要一个 `TokenSource` 对象,这个对象一般是语法解析器 `Lexer`,  
语法 `[Grammer]` 的词法解析器是 `[Grammar]Lexer` 类, 构造该类需要一个 `CharStream` 对象, 该对象可通过它的静态方法从一个标准流或一个文件名构造.  

总结若要构造 `[Grammar]Parser` 类, 需要依次构造如下对象:

1. `CharStream`
2. `[Grammar]Lexer`
3. `CommonTokenStream`
4. `[Grammar]Parser`

`[Grammar]Parser` 类提供了一系列以语法规则命名的方法,调用这些方法会使得解析器从该规则开始解析输入流,并返回该规则的一个 `ParserRuleContext`.

### 遍历

通过 *监听器* 遍历语法树时, 需要一个 `ParamTreeWalker`, 使用它的 `walk` 方法传入 *监听器* 和 由解析器获得语法树, 即可使用监听器遍历所有节点.

*访问器* 可以带有一个类型参数,作为它的访问器方法的返回值. 通过语法树的 `accept` 方法传入访问器即可遍历.

