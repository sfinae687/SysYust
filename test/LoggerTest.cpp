/// @file 日志器测试程序

#include <gtest/gtest.h>
#include <iostream>
#include <cstdio>

#include <filesystem>

#include "Logger.h"
#include "FileLogger.h"
#include "StringLogger.h"
#include "Trace.h"

namespace fs = std::filesystem;

using SysYust::Logger;
using SysYust::LoggerLevel;

std::FILE *getFile(const char *path) {
    std::error_code ec;
    fs::path logFile(path);
    auto logDir = logFile.parent_path();

    // 文件已经存在，那么打开并返回
    if (fs::exists(logFile)) {
        if (!fs::is_regular_file(logFile)) {
            std::cerr << path << "is not a regular file" << std::endl;
            std::exit(EXIT_FAILURE);
        } else {
            return std::fopen(path, "a");
        }
    }

    // 否则创建它的父目录们
    if (!fs::exists(logDir)) {
        fs::create_directories(logDir, ec);
        if (ec) {
            std::cerr << ec.message() << std:: endl;
            std::exit(EXIT_FAILURE);
        }
        return std::fopen(path, "a");
    }
}

TEST(LoggerTest, FileLoggerTest) {
    std::FILE *logfile = getFile("log/FileTest.log");
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
    auto TopTrace = SysYust::Trace::TopTrace;
    ASSERT_EQ(i+1, TopTrace->depth);
    LOG_TRACE(TopTrace->getArgRecords());
    ASSERT_EQ(TopTrace, SysYust::Trace::TopTrace);
    if (i < n) {
        return Recurrence(n, i+1);
    } else {
        return;
    }
}

TEST(LoggerTest, TraceTest) {
    Logger::setLogger(new SysYust::FileLogger(getFile("log/FileTest.log"), "a"));
    auto current = SysYust::Trace::TopTrace;
    Recurrence(10);
    ASSERT_EQ(SysYust::Trace::TopTrace, current) << "Unable to destruct Trace correctly";
}

void StackPrinterRe(int n, int i=0) {
    if (i < n) {
        LOG_STACK();
        ASSERT_EQ(i+1, SysYust::Trace::TopTrace->depth) << "depth untracked";
        StackPrinterRe(n, i+1);
    } else {
        return;
    }
}

TEST(LoggerTest, StackPrinterTest) {
    Logger::setLogger(new SysYust::FileLogger(getFile("log/FileTest.log"), "a"));
    StackPrinterRe(10);
}
