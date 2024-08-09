# SysYust

## 关于三方库的使用说明

目录 `lib` 中的所有文件均为第三方库文件的源代码副本。本项目引用了以下库

- [{fmt}](https://github.com/fmtlib/fmt)
- [argparse](https://github.com/p-ranav/argparse)
- [range-v3](https://github.com/ericniebler/range-v3?tab=readme-ov-file)

项目开发过程中使用了 `Google Test` 但最终提交中不包含其代码及测试文件。

前端使用 Antlr4 生成，提交文件中包含了语法文件的定义 `src/frontend/SysY.g4`. 

生成的目标再本地生成后随着源代码提交，位于 `src/frontend/generated`. `include/antlr4_target` 有其中头文件的副本.

## 描述

