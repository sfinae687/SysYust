//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_CODE_H
#define SYSYUST_CODE_H

#include "IR/CodeContext.h"
#include "IR/Procedure.h"
#include "IR/InitInfo.h"

namespace SysYust::IR {

    class Code {
        using global_var_value_type = std::unordered_map<var_symbol, InitInfo>; ///< @todo 需要一个描述初始化列表的类型
    public:

        /**
         * @brief 注册一个函数
         * @todo 用例与定义的维护：变量符号，函数
         * @param name 函数的名称
         * @param info 函数的描述信息
         * @return 注册的函数的 Procedure 引用
         */
        Procedure& setup_procedure(const func_symbol& name, func_info info);

        std::list<Procedure> & procedures();
        const std::list<Procedure>& procedures() const;

        /**
         * @brief 添加一个全局符号并指定初始值
         * @note 正确指定符号类型是必要的
         * @details 如果符号不存在会自动创建，如果符号类型和初始值类型不匹配，如果符号已存在以符号类型为准，如果符号不存在以初始值类型为准。自动的
         * 类型补充仅使用整数和浮点类型.
         * @param var 符号的名称
         * @param init 符号的初始值，默认为 undef 标签
         */
        void set_var(var_symbol var, InitInfo init = {});
        /**
         * @brief 删除一个全局变量符号
         */
        void unset_var(var_symbol var);
        /**
         * @brief 检测一个符号是否是全局变量符号
         */
        bool is_global_var(var_symbol var) const;

        CodeContext context{};
    private:
        std::list<Procedure> _all_procedure{};
        global_var_value_type global_var_value{};
    };

} // IR

#endif //SYSYUST_CODE_H
