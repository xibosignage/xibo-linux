#pragma once

#include "DateTime.hpp"

class DateTimeProvider
{
public:
    static DateTime now();
    static DateTime nowUtc();
    static DateTime fromTimeT(const std::time_t& timestamp);
    static DateTime utcFromTimeT(const std::time_t& timestamp);

    static std::string toString(const DateTime& dt, const char* format);
    static std::string toString(const DateTime& dt);
    static DateTime fromString(const std::string& str);
    static DateTime fromIsoExtendedString(const std::string& str);

};
