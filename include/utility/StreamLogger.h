//
// Created by LL06p on 24-7-12.
//

#ifndef SYSYUST_STREAMLOGGER_H
#define SYSYUST_STREAMLOGGER_H

#include <iostream>

#include "Logger.h"

namespace SysYust {

    class StreamLogger : public Logger {
    public:
        explicit StreamLogger(std::ostream &s);
    protected:
        void write(std::string_view record) override;
    private:
        std::ostream &_stream;
    };

}

#endif //SYSYUST_STREAMLOGGER_H
