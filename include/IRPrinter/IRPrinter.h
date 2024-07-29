/// IR 的精美打印

#ifndef SYSYUST_IRPRINTTER_H
#define SYSYUST_IRPRINTTER_H

#include "IR/Instruction.h"
#include "IR/SymbolUtil.h"
#include "IR/TypeUtil.h"
#include "IR/Code.h"
#include "fmt/core.h"

namespace SysYust::IR {

/* ------------ TypeUtil ------------ */

/// Type formatter
std::string format_as(const Type &type);

/* ----------- SymbolUtil ----------- */

/// var_symbol formatter
std::string format_as(const var_symbol &var);
/// im_symbol formatter
std::string format_as(const im_symbol &im);
/// operant formatter
std::string format_as(const operant &op);

/* --------- InstructionUtil -------- */

/// instruct_type formatter
std::string format_as(instruct_type type);
/// instruct_cate formatter
std::string format_as(instruct_cate type);
/// instruction formatter
std::string format_as(const instruction &inst);

/* ----------- BasicBlock ----------- */

/// BasicBlock formatter
std::string format_as(const BasicBlock *bb);

/* ----------- ControlFlow ---------- */

/// ControlFlow formatter
std::string format_as(const ControlFlow &cfg);

/* -------------- Code -------------- */

std::string format_as(const InitInfo &init);

std::string format_as(const func_symbol &func);

std::string format_as(const Procedure &proc);

std::string format_as(const Code &code);

}  // namespace SysYust::IR

#endif  // SYSYUST_IRPRINTTER_H