#pragma once

#include <istream>

enum class LoggingLevel
{
    Trace,
    Debug,
    Info,
    Error
};

std::istream& operator>>(std::istream& in, LoggingLevel& level);
std::ostream& operator<<(std::ostream& out, LoggingLevel level);
