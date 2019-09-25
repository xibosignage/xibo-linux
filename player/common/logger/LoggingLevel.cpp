#include "LoggingLevel.hpp"
#include "Utils.hpp"

template <>
inline std::string Utils::toString(LoggingLevel val)
{
    switch (val)
    {
        case LoggingLevel::Trace: return "Trace";
        case LoggingLevel::Debug: return "Debug";
        case LoggingLevel::Info: return "Info";
        case LoggingLevel::Error: return "Error";
    }

    return "unknown";
}

std::istream& operator>>(std::istream& in, LoggingLevel& level)
{
    int temp;
    in >> temp;
    level = static_cast<LoggingLevel>(temp);
    return in;
}

std::ostream& operator<<(std::ostream& out, LoggingLevel level)
{
    return out << static_cast<int>(level);
}
