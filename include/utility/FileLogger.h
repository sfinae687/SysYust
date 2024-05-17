/// @file FileLogger 定义

#ifndef SYSYUST_UTILITY_FILELOGGER_H
#define SYSYUST_UTILITY_FILELOGGER_H

#include <cstdio>

#include "logger.h"

namespace SysYust {

    class FileLogger: public logger {
    public:
        /**
         * @brief 默认构造，以标准错误输出流构造
         */
        FileLogger();
        /**
         * @brief 以一个输出文件和是否自动释放构造
         * @param logFile 输出的文件，不可为空，错误检查由主调方进行
         * @param autoRelease 是否在析构函数自动析构日志文件
         */
        explicit FileLogger(std::FILE *logFile, bool autoRelease = true);
        ~FileLogger() override;
    protected:
        void write(std::string_view record) override;
    private:
        std::FILE *_logFile;
        bool _autoRelease;
    };

}

#endif //SYSYUST_UTILITY_FILELOGGER_H
