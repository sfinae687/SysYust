# SySYust

## 关于三方库的使用说明

目录 `lib` 中的所有文件均为第三方库文件。本项目引用了以下库

- `clutchlog`
- `{fmt}`
- `argparse`

项目开发过程中使用了 `Google Test` 但最终提交中不包含其代码及测试文件。

## 描述

SySY2022编译器项目

Branch:pre_impl 可行性实验主分支 -> buildsystem 构建系统搭建

## 构建系统

### 目标

- Document 调用Doxygen生成文档，可选的
- Antlr4_SysY_Generate 自定义目标，用于调用Antlr4生成目标文件
- Antlr4_SysY 常规库目标