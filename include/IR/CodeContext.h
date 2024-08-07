//
// Created by LL06p on 24-7-24.
//

#ifndef SYSYUST_CODECONTEXT_H
#define SYSYUST_CODECONTEXT_H

#include <set>
#include <unordered_set>

#include "IR/SymbolUtil.h"

namespace SysYust::IR {

    /**
     * @brief IR 程序级别的上下文，包含了全局变量和函数的符号信息
     */
    class CodeContext {
        using func_set_t = std::unordered_map<func_symbol, func_info>;
        using global_var_set = std::unordered_set<var_symbol>;
    public:

        /**
         * @brief 获取指定函数符号的返回值类型
         */
        [[nodiscard]] const Type* get_fun_type(const func_symbol& sym) const;
        /**
         * @brief 获取指定函数符号的函数描述
         */
        [[nodiscard]] func_info get_func(const func_symbol& sym) const;
        /**
         * @brief 设定指定函数符号的函数描述
         */
        void set_func(const func_symbol& sym, func_info func);

        /**
         * @brief 添加一个全局变量符号
         * @note 添加全局符号时请注意添加正确的符号类型
         */
        void addGlobal(const var_symbol &sym);
        /**
         * @brief 删除一个全局变量符号
         */
        void removeGlobal(const var_symbol &sym);
        /**
         * @brief 获取一个全局变量的类型
         */
        const Type * globalType(const var_symbol &sym);
        /**
         * @brief 检测指定变量符号是否是全局变量
         */
        [[nodiscard]] bool isGlobal(const var_symbol& sym) const;

        /**
         * @brief 获取全局变量集合
         */
        const global_var_set& global_vars() const;

        // 在 Code 对象中获取/设置全局符号的初始值

    private:
        func_set_t func_set{};
        global_var_set _global_vars{};
    };

} // IR

#endif //SYSYUST_CODECONTEXT_H
