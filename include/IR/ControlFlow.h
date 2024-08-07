//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_CONTROLFLOW_H
#define SYSYUST_CONTROLFLOW_H

#include <list>

#include "IR/BasicBlock.h"

namespace SysYust::IR {

    /**
     * @brief 控制流图
     * @details 具有一个拥有前控制流图所有基本块的节点池。节点之间的连接由基本块本身维护. 提供指针的形式访问，内存管理由 ControlFlow 类内部维护
     * 在基本块被从 ControlFlow 中移除后所有对它的引用都将变为悬垂的。
     */
    class ControlFlow {
    public:

        /**
         * @brief 基本块中对符号的用例的记录类
         */
        struct UsageInBlock {

        };

        // 节点
        /**
         * @brief 添加一个节点
         * @detils 如果当前基本块列表为空那么该节点会被设置为头节点
         * @return 添加节点的指针
         */
        BasicBlock* add();
        /**
         * @brief 移除一个节点
         * @details 实现仅会删除当前的节点，不会处理其他对该节点引用的节点
         * @return 返回 true 仅当成功删除时
         */
        bool erase(BasicBlock *);
        /**
         * @brief 获取当前的头节点
         */
        [[nodiscard]] BasicBlock* head() const;
        /**
         * @brief 设置当前的头节点
         */
        void setHead(BasicBlock *);

        const std::list<BasicBlock>& all_nodes() const;

        ControlFlow() = default;
        ControlFlow(ControlFlow &) = delete;
        ControlFlow &operator=(ControlFlow &) = delete;

    private:
        BasicBlock *_head = nullptr;
       std::list<BasicBlock> _nodes{};
    };

} // IR

#endif //SYSYUST_CONTROLFLOW_H
