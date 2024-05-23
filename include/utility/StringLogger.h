//
// Created by LL06p on 24-5-17.
//

#ifndef SYSYUST_UTILITY_STRINGLOGGER_H
#define SYSYUST_UTILITY_STRINGLOGGER_H

#include <utility>
#include <string>

#include "Logger.h"

namespace SysYust {

    /**
     * @brief 将日志记录写入字符串缓存
     */
    class StringLogger: public Logger {
    public:
        /**
         * @brief 默认构造函数，以一个空字符串构造缓存区
         */
        StringLogger();

        /**
         * @brief 获取缓存的字符串
         * @return 缓存的字符串的副本
         */
        std::string getString();
        /**
         * @brief 以交换的方式获取缓存区字符串
         * @param str 被交换的字符串对象
         */
        void getString(std::string &str);
    protected:
        void write(std::string_view record) override;
    private:
        std::string _buffer;
    };

} // SysYust

#endif //SYSYUST_UTILITY_STRINGLOGGER_H
