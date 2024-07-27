//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_PROCEDURE_H
#define SYSYUST_PROCEDURE_H

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
         * @brief 检测当前变量符号名是否是形参
         * @return 当给定符号名是形参时，返回true
         */
        bool is_param(var_symbol name);
        /**
         * @brief 获取当前变量符号形参的索引
         * @return 形参的索引，不存在时返回 -1
         */
        std::size_t param_index(var_symbol name);

    private:
        func_symbol _name;
        struct ProcedureContext _context;
        ControlFlow _graph{};
    };

} // IR

#endif //SYSYUST_PROCEDURE_H
