//
// Created by LL06p on 24-5-17.
//

#include "StringLogger.h"

namespace SysYust {
    StringLogger::StringLogger()
    :_buffer() {

    }

    std::string StringLogger::getString() {
        return _buffer;
    }

    void StringLogger::getString(std::string &str) {
        using std::swap;
        swap(_buffer, str);
    }

    void StringLogger::write(std::string_view record) {
        _buffer.append(record);
    }
} // SysYust