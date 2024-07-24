//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_BASICBLOCK_H
#define SYSYUST_BASICBLOCK_H

#include "IR/Instruction.h"

namespace SysYust::IR {

    /**
     * @brief 一个基本块是一组一定连续执行的命令
     * @details 提供了存储一个基本块所有指令序列的机制，并提供了快速访问基本快出口的方法。
     */
    class BasicBlock {
    public:
        using instruction_list_t = std::list<instruction>;
        using iterator = instruction_list_t::iterator;
        using const_iteraor = instruction_list_t::const_iterator;
        using reverse_iterator = instruction_list_t::reverse_iterator;
        using const_reverse_iterator = instruction_list_t::const_reverse_iterator;
        using value_type = instruction_list_t::value_type;
        using reference = instruction_list_t::reference;
        using const_reference = instruction_list_t::const_reference;
        using const_pointer = instruction_list_t::const_pointer;

        // 密封和出口操作

        /**
         * @brief 将基本块密封，标志它已经完成了所有构造和添加
         * @details 已经密封的基本块最后一条指令一定是gateway指令.不在能向其添加指令，但是可以改变它的后续基本块
         */
        void seal();
        /**
         * @brief 检测当前基本块是否是密封的
         */
        [[nodiscard]] bool is_sealed() const;
        /**
         * @brief 出口指令，仅当基本块已经密封才可使用。
         */
        [[nodiscard]] gateway_inst gateway() const;
        /**
         * @brief 设置出口的跳转目标
         */
        void setTarget(BasicBlock *t);
        /**
         * @brief 设置分支的否则跳转目标
         */
        void setElseTarget(BasicBlock *t);
        /**
         * @brief 获取跳转目标
         */
        BasicBlock* nextBlock();
        /**
         * @brief 获取分支的否则跳转目标
         */
        BasicBlock* elseBlock();
        /**
         * @brief 获取前驱节点
         */
        const std::vector<BasicBlock*>& prevBlocks();

        // 指令序列操作

        /**
         * @brief 在序列最后添加添加一条指令
         */
        void push(instruction item);
        /**
         * @brief 删除序列最后一条指令
         */
        void pop();
        /**
         * @brief 在特定位置插入一条指令
         */
        iterator insert(iterator pos, const_reference value);
        /**
         * @brief 删除特定位置的指令
         */
        iterator erase(iterator pos);
        /**
         * @brief 删除一个迭代器区间
         */
        iterator erase(iterator first, iterator last);
        /**
         * @brief 获取序列中的最后一条指令
         */
        instruction& back() ;
        /**
         * @brief 获取序列最后一条指令，常量使用
         */
        [[nodiscard]] const instruction& back() const;

        // 迭代器方法

        auto begin() {
            return instruction_list.begin();
        }
        [[nodiscard]] auto begin() const {
            return instruction_list.begin();
        }
        [[nodiscard]] auto cbegin() const {
            return instruction_list.cbegin();
        }
        auto end() {
            return instruction_list.end();
        }
        [[nodiscard]] auto end() const {
            return instruction_list.end();
        }
        [[nodiscard]] auto cend() const {
            return instruction_list.cend();
        };

    private:

        bool sealed = false;
        std::vector<BasicBlock*> _prevBlock;
        BasicBlock *trueTarget = nullptr;
        BasicBlock *falseTarget = nullptr;
        instruction_list_t instruction_list{};

        /// @todo 用例维护
    };

} // IR

#endif //SYSYUST_BASICBLOCK_H
