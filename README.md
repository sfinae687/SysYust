# SysYust

## 关于三方库的使用说明

目录 `lib` 中的所有文件均为第三方库文件。本项目引用了以下库

- `{fmt}`
- `argparse`

项目开发过程中使用了 `Google Test` 但最终提交中不包含其代码及测试文件。

## 描述

SySY2022编译器项目

## 构建系统

### 目标

本项目目标

- Document 调用Doxygen生成文档，可选的
- Antlr4_SysY_Generate 自定义目标，用于调用Antlr4生成目标文件
- Antlr4_SysY 常规库目标

第三方库目标

- `fmt::fmt` 库目标, 来自 `{fmt}`
- `GTest::gtest_main` 包含 `main` 函数的库目标, 来自 `Google Test` 框架
- `argparse` 库目标, 来自 `argparse`
- `Doxygen::doxygen` 可执行目标, 指向 `doxygen` 可执行程序