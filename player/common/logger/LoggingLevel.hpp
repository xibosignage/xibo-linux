#pragma once

#include <istream>

enum class LoggingLevel
{
    Trace,
    Debug,
    Info,
    Error
};


inline std::istream& operator>> (std::istream& in, LoggingLevel& level)
{
    int temp;
    in >> temp;
    level = static_cast<LoggingLevel>(temp);
    return in;
}

inline std::ostream& operator<< (std::ostream& out, LoggingLevel level)
{
    return out << static_cast<int>(level);
}
