/// @file 日志器测试程序

#include <gtest/gtest.h>
#include <iostream>
#include <cstdio>

#include "Logger.h"
#include "FileLogger.h"
#include "StringLogger.h"

using SysYust::Logger;
using SysYust::LoggerLevel;

TEST(LoggerTest, FileLoggerTest) {
    std::FILE *logfile = std::fopen("log/logTest.txt", "a");
    if (logfile != nullptr) {
        Logger::setLogger(new SysYust::FileLogger(logfile));
    } else {
        perror("Open Log File failed");
        FAIL() << "Unable to open log file";
    }
    LOG_INFO("Hello");
    LOG_DEBUG("World");
}

TEST(LoggerTest, StringLoggerTest) {
    Logger::setLogger(new SysYust::StringLogger());
    LOG_INFO("HelloWorld");
    LOG_INFO("ByeWorld");
    std::cout << static_cast<SysYust::StringLogger&>(SysYust::Logger::getLogger()).getString() << std::endl;
}
