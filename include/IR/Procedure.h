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
        Procedure(CodeContext &code, func_symbol name);
        /**
         * @brief 构造 Procedure，同时在构造函数内向上下文注册信息
         */
         Procedure(CodeContext &code, func_symbol name, func_info info);

        // 控制流
        ControlFlow& getGraph();

        // 附加信息
        func_symbol name() const;
        /**
         * @brief 获取当前函数对象关联的描述信息,常量使用
         */
        func_info info() const;
        /**
         * @brief 获取函数的返回类型
         */
        const Type* type() const;

        // 上下文

        /**
         * @brief 获取当前函数的上下文对象
         */
        ProcedureContext* context();
        /**
         * @brief 获取当前函数的上下文对象
         */
        const ProcedureContext* context() const;

    private:
        func_symbol _name;
        ProcedureContext _context;
        ControlFlow _graph{};
    };

} // IR

#endif //SYSYUST_PROCEDURE_H
