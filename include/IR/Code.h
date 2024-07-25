//
// Created by LL06p on 24-7-20.
//

#ifndef SYSYUST_CODE_H
#define SYSYUST_CODE_H

#include "CodeContext.h"
#include "Procedure.h"

namespace SysYust::IR {

    class Code {
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


        CodeContext context{};
    private:
        std::list<Procedure> _all_procedure{};
    };

} // IR

#endif //SYSYUST_CODE_H
