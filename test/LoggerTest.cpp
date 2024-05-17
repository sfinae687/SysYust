/// @file 日志器测试程序

#include <gtest/gtest.h>
#include <iostream>
#include <cstdio>

#include "Logger.h"
#include "FileLogger.h"
#include "StringLogger.h"
#include "Trace.h"

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
    std::cout << dynamic_cast<SysYust::StringLogger&>(SysYust::Logger::getLogger()).getString() << std::endl;
}

void Recurrence(int n, int i = 0) {
    TRACEPOINT(i, n);
    auto & TopTrace = SysYust::Trace::TopTrace;
    ASSERT_EQ(i+1, TopTrace->depth);
    LOG_TRACE(TopTrace->getArgRecords());
    if (i != n) {
        return Recurrence(n, i+1);
    } else {
        return;
    }
}

TEST(LoggerTest, TraceDepthTest) {
    TRACEPOINT();
    Logger::setLogger(new SysYust::FileLogger(std::fopen("log/TraceLog.log", "a")));
    auto current = SysYust::Trace::TopTrace;
    Recurrence(10);
    ASSERT_EQ(SysYust::Trace::TopTrace, current) << "Unable to destruct Trace correctly";
}
