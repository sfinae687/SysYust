/// @file 函数信息条目的实现文件

#include "AST/Env/FuncInfo.h"

namespace SysYust::AST {
    std::strong_ordering FuncInfo::operator<=>(const FuncInfo &oth) const {
        return name <=> oth.name;
    }
} // AST