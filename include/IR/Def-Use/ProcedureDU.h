//
// Created by LL06p on 24-7-28.
//

#ifndef SYSYUST_PROCEDUREDU_H
#define SYSYUST_PROCEDUREDU_H

#include <map>
#include <memory>

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

#include "IR/Procedure.h"
#include "IR/ContextualMixin.h"
#include "IR/Def-Use/DU_Util.h"

namespace SysYust::IR {

    namespace ranges = ::ranges;
    namespace views = ranges::views;

    ///< 如果设置此符号，所有立即数符号都会被函数本地保存, nullptr被视为声明立即数的基本块，否则会被立即忽略
    constexpr bool save_constant_flag = true;

    /**
     * @brief 保存了整个函数的用例信息，为保存用例信息的最顶层上下文(所有无法在本地找到的符号名,视为全局变量名)
     * @details 这个类里保存了函数中所有符号名字和指令的信息，这包括了名字到定义的映射，定义的信息，指令到指令描述的映射，符号的用例信息。
     * 用例信息使用有序列表保存，相同基本块中的用例相邻。
     */
    class ProcedureDU {
    public:

#if 0
        friend class usage_iterator;
        class usage_iterator {
            friend class ProcedureDU;
        public:
            usage_iterator(const usage_iterator &) = default;

            usage_iterator& operator++ () {
                ++_iter;
                return *this;
            }
            usage_iterator operator++ (int) {
                auto rt = *this;
                ++*this;
                return rt;
            }
            usage_iterator& operator-- () {
                --_iter;
                return *this;
            }
            usage_iterator operator-- (int) {
                auto rt = *this;
                --*this;
                return rt;
            }
            UsageEntry& operator* () {
                return *_iter;
            }
            const UsageEntry& operator* () const {
                return *_iter;
            }
            UsageEntry* operator-> () {
                return &*_iter;
            }
            const UsageEntry* operator-> () const {
                return &*_iter;
            }

            BasicBlock* block() {
                return _iter->block;
            }
            [[nodiscard]] const BasicBlock* block() const {
                return _iter->block;
            }
            instruction& inst() {
                return *_iter->inst;
            }
            [[nodiscard]] const instruction& inst() const {
                return *_iter->inst;
            }


        private:
            explicit usage_iterator(usage_list_type::iterator iter)
                : _iter(iter)
            {

            }
            usage_list_type::iterator _iter;
        };
#endif

        using usage_range = ranges::subrange<usage_list_type::iterator>;

        explicit ProcedureDU(Procedure &context);

        // 指令处理

        /**
         * @brief 添加一条指令到图中，同时更新定义-使用图
         */
        void add_inst(BasicBlock *block, BasicBlock::iterator inst);
        /**
         * @brief 从图中删除一条指令
         */
        bool remove_inst(BasicBlock::iterator inst);
        /**
         * @brief 重新设置已有的指令，如果该指令不存在则无行动。
         * @details 这一操作会使得所有对该指令产生的定义的用例被更新，指令中的参数被重新设置.该方法会使用 inst 中的上下文定位基本块和指令位置，
         * 给出的指令仅提供内容
         */
        void set_inst(BasicBlock::iterator inst, const instruction &nInst);

        /**
         * @brief 获取指令关联的定义-用例信息
         */
        DUInst& instDUInfo(BasicBlock::iterator inst);

        // 定义处理

        /**
         * @brief 添加函数形参名字
         */
        void add_func_param(const var_symbol& var);
        /**
         * @brief 添加基本块形参名字
         */
        void add_block_param(BasicBlock* block, const var_symbol& var);

        /**
         * @brief 重命名一个定义
         */
        void rename_define(const value_type &val, const value_type &nVal);

        /**
         * @brief 移除一个符号的定义
         */
        bool remove_define(const value_type& val);

        /**
         * @brief 获取一个变量名的定义信息,如果该符号不在本地那么会被视为全局变量
         * @details 全局变量的信息不存储在本地，如果出现这种情况返回的实际上是生成的一个对象。
         */
        [[nodiscard]] DefineEntry defined_entry(value_type var) const;
        /**
         * @brief 获取本地存储的符号定义信息
         */
        const DefineEntry* local_defined(const value_type& var) const;
        /**
         * @brief 获取本地存储的符号定义信息
         */
        DefineEntry* local_defined(const value_type& var);

        // 用例处理

        /**
         * @brief 获取指令变量名在特定基本块中的用例信息
         */
        usage_range usage_in_block(const value_type& var, BasicBlock *block);
        /**
         * @brief 获取在指定变量名的用例信息
         */
        usage_ref_list_type usage_set(const value_type& var);

    private:
        std::pair<usage_list_type &, usage_list_type::iterator>
        usage_insert_pos(const value_type& var, BasicBlock *block, BasicBlock::iterator inst);

        usage_pointer add_usage(BasicBlock *block, BasicBlock::iterator inst, value_type var, std::size_t ind);
        void erase_usage(usage_pointer ptr);

    private:
        CodeContext &global;
        Procedure &procedure;

        mutable define_list_type _define_list{};
        indexed_usage_list_type _usage_list{};
        DUInst_list_type _inst_list{};
    };

} // IR

#endif //SYSYUST_PROCEDUREDU_H
