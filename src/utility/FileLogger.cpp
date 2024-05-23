/// @file FileLogger 实现

#include <cassert>

#include "FileLogger.h"

SysYust::FileLogger::FileLogger()
: FileLogger(stderr, false){

}

SysYust::FileLogger::FileLogger(std::FILE *logFile, bool autoRelease)
: _logFile(logFile)
, _autoRelease(autoRelease) {
    assert(logFile != nullptr);
}

SysYust::FileLogger::~FileLogger() {
    if (_autoRelease) {
        std::fclose(_logFile);
    }
}

void SysYust::FileLogger::write(std::string_view record) {
    fputs(record.data(), _logFile);
}

