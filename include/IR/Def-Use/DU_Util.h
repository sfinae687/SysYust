//
// Created by LL06p on 24-7-28.
//

#ifndef SYSYUST_DU_UTIL_H
#define SYSYUST_DU_UTIL_H

#include <map>
#include <list>

#include "IR/SymbolUtil.h"
#include "IR/TypeUtil.h"
#include "IR/Instruction.h"
#include "IR/BasicBlock.h"
#include "IR/ContextualMixin.h"

namespace SysYust::IR {

    enum class SymbolDefineType {
        block, ///< 块定义的符号信息
        block_param, ///< 基本块参数符号
        func_param, ///< 函数参数符号
        global, ///< 全局变量信息
        external, ///< 在外部定义的符号，比如在基本块意义下，其他基本块的符号
        meaningless, ///< 无意义符号定义类型，用于弃用的符号（比如，无返回值函数的调用的返回的符号）
        builtin, ///< 库函数的名字和立即数可以被认为是预置的
        unknown,
    };

    struct DefineEntry;
    struct UsageEntry;
    struct DUInst;

    using value_type = operant;

    using define_list_type = std::map<value_type, DefineEntry>;
    using usage_list_type = std::list<UsageEntry>; ///< 用例的列表

    using usage_pointer = usage_list_type::iterator;

    using usage_ref_list_type = std::vector<std::reference_wrapper<UsageEntry>>;
    using indexed_usage_list_type = std::map<value_type, std::map<BasicBlock*, usage_list_type>>;

    using DUInst_list_type = std::map<instruction*, DUInst>;

    struct DefineEntry {

        explicit DefineEntry(var_symbol var, DUInst *du = nullptr, SymbolDefineType type = SymbolDefineType::unknown);
        explicit DefineEntry(var_symbol var, BasicBlock *block);
        explicit DefineEntry(im_symbol im);

        // 数据成员

        operant defined; ///< 被定义的符号的名字
        SymbolDefineType define_type = SymbolDefineType::meaningless; ///< 符号的定义种类, 被记录下来的符号只有立即数符号，块的参数和变量，函数的参数
        std::variant<std::monostate, DUInst*, BasicBlock*> inst_context = std::monostate{};

        // 访问器方法

        [[nodiscard]] DUInst* du_inst() const;
        [[nodiscard]] std::vector<usage_pointer> & dependence() const; ///< 所使用的符号的名字
        [[nodiscard]] BasicBlock *block() const; ///< 定义该符号的基本块, 可能为空
        [[nodiscard]] std::optional<BasicBlock::iterator> inst() const; ///< 定义这个符号的指令

        // 比较

        friend auto operator< (const DefineEntry &lhs, const DefineEntry &rhs) {
            return lhs.defined < rhs.defined;
        }
        friend auto operator== (const DefineEntry &lhs, const DefineEntry &rhs) {
            return lhs.defined == rhs.defined;
        }

    };
    struct UsageEntry {

        [[nodiscard]] operant opr() const;
        void setOpr(const operant& nOpr) const;

        DefineEntry *defined; ///< 被使用的变量的定义
        BasicBlock *block; ///< 用例关联的指令所在的基本块
        BasicBlock::iterator inst; ///< 用例关联的指令
        std::size_t index; ///< 在使用该变量的指令中的位于的参数的序号
    };

    struct DUInst {
        BasicBlock *block;
        BasicBlock::iterator inst;
        std::vector<usage_pointer> args;
        DefineEntry *defineInfo;
    };

}

#endif //SYSYUST_DU_UTIL_H
