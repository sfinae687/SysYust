/// @file 日志器测试程序

#include <gtest/gtest.h>

#include <cstdio>

#include "logger.h"
#include "FileLogger.h"

using SysYust::logger;
using SysYust::LoggerLevel;

TEST(LoggerTest, FileLoggerTest) {
    std::FILE *logfile = std::fopen("log/logTest.txt", "a");
    if (logfile != nullptr) {
        logger::setLogger(new SysYust::FileLogger(logfile));
    } else {
        perror("Open Log File failed");
        FAIL() << "Unable to open log file";
    }
    LOG_INFO("Hello");
    LOG_DEBUG("World");
}
