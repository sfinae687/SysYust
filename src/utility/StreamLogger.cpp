//
// Created by LL06p on 24-7-12.
//

#include "StreamLogger.h"

namespace SysYust {

    StreamLogger::StreamLogger(std::ostream &s)
    : Logger()
    , _stream(s) {

    }

    void StreamLogger::write(std::string_view record) {
        _stream << record << std::endl;
    }
}