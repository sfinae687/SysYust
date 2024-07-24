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
         * @brief 检测指定变量符号是否是全局变量
         */
        [[nodiscard]] bool is_global(const var_symbol& sym) const;

    private:
        func_set_t func_set{};
        global_var_set global_vars{};
    };

} // IR

#endif //SYSYUST_CODECONTEXT_H
