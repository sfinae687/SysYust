//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_PROCEDURE_H
#define SYSYUST_PROCEDURE_H

#include "IR/Def-Use/ProcedureDU.h"
#include "IR/ControlFlow.h"
#include "CodeContext.h"
#include "ProcedureContext.h"

namespace SysYust::IR {

    /**
     * @brief 过程/函数
     * @details 主要内容是一个控制流图，也提供了便于访问其类型和附加信息的方法。Procedure 的构建需要 code 的上下文对象. 完整的 Procedure 构建
     * 还应包含了在 code 上下文中注册函数的相关信息。
     */
    class Procedure {
    public:
        /**
         * @brief 构造 Procedure，不自动向上下文注册信息
         */
        Procedure(CodeContext &code, func_symbol name, std::vector<var_symbol> params = {});
        /**
         * @brief 构造 Procedure，同时在构造函数内向上下文注册信息
         */
        Procedure(CodeContext &code, func_symbol name, func_info info, std::vector<var_symbol> params = {});

        Procedure(Procedure &) = delete;

        // 控制流
        ControlFlow& getGraph();

        // 附加信息
        [[nodiscard]] func_symbol name() const;
        /**
         * @brief 获取当前函数对象关联的描述信息,常量使用
         */
        [[nodiscard]] func_info info() const;
        /**
         * @brief 获取函数的返回类型
         */
        [[nodiscard]] const Type* type() const;

        // 上下文

        /**
         * @brief 获取当前函数的上下文对象
         */
        ProcedureContext* context();
        /**
         * @brief 获取当前函数的上下文对象
         */
        [[nodiscard]] const ProcedureContext* context() const;

        // 形参信息

        /**
         * @brief 添加一个符号名字作为该函数的形参
         */
        void add_param(var_symbol name);
        /**
         * @brief 从形参集合里移除该符号名
         * @return 当成功移除时返回 true。
         */
        bool remove_param(var_symbol name);
        /**
         * @brief 清楚所有的形参
         */
        void clear_param();
        /**
         * @brief 对集合内的形参重新添加到函数形参追踪中
         * @details 这个操作不会删除之前添加的形参追踪
         */
        void resync_param();
        /**
         * @brief 检测当前变量符号名是否是形参
         * @return 当给定符号名是形参时，返回true
         */
        bool is_param(var_symbol name);
        /**
         * @brief 获取当前变量符号形参的索引
         * @return 形参的索引，不存在时返回 -1
         */
        std::size_t param_index(var_symbol name);

        // 定义用例追踪,以下方法只更改追踪信息，不修改基本块实际的存储内容。

        /**
         * @ 添加一条已经添加到基本块的语句的用例-定义追踪
         */
        void track_inst(BasicBlock *block, BasicBlock::iterator inst_ptr);
        /**
         * @brief 删除一条有语句的用例定义追踪
         */
        void untrack_inst(BasicBlock::iterator inst_ptr);
        /**
         * @brief 重新设置一条语句的追踪信息
         * @details 该方法 **不会通过基本块** 访问旧的语句信息，并且会重新设置迭代器所指向的指令为给出的指令。
         */
        void retrack_inst(BasicBlock::iterator inst_ptr, const instruction &nInst);
        /**
         * @brief 添加一条基本块参数的追踪信息
         * @details 不会实际上修改基本块的参数，只会更改追踪信息
         */
        void track_block_arg(BasicBlock *block, var_symbol arg);

        /**
         * @brief 删除一个变量的追踪信息
         */
        void untrack_symbol(IR::value_type sym);

        /**
         * @brief 将一个变量的追踪信息转移到另一个上
         */
        void retrack_symbol_as(IR::value_type sym, IR::value_type nSym);

        /**
         * @brief 获取定义-用例信息
         */
        const ProcedureDU& du_info() const;
        ProcedureDU & du_info();



    private:
        func_symbol _name;
        struct ProcedureContext _context;
        ControlFlow _graph{};
        ProcedureDU _du;
    };

} // IR

#endif //SYSYUST_PROCEDURE_H
