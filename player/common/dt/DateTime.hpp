#pragma once

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

class DateTime
{
public:
    using Date = boost::gregorian::date;
    using Time = boost::posix_time::time_duration;
    using Seconds = boost::posix_time::seconds;
    using Minutes = boost::posix_time::minutes;
    using Hours = boost::posix_time::hours;

    DateTime() = default;
    DateTime(const Date& date, const Time& td);

    static DateTime now();
    static DateTime nowUtc();
    static DateTime localFromTimestamp(const std::time_t& timestamp);
    static DateTime utcFromTimestamp(const std::time_t& timestamp);

    static std::string toString(const DateTime& dt, const char* format);
    static std::string toString(const DateTime& dt);
    static DateTime fromString(const std::string& str);
    static DateTime fromIsoExtendedString(const std::string& str);

    bool isValid() const;

private:
    explicit DateTime(const boost::posix_time::ptime& ptime);

    friend DateTime operator+(const DateTime& first, const DateTime::Hours& hours);
    friend DateTime operator+(const DateTime& first, const DateTime::Minutes& mins);
    friend DateTime operator+(const DateTime& first, const DateTime::Seconds& secs);
    friend DateTime::Time operator-(const DateTime& first, const DateTime& second);
    friend DateTime operator-(const DateTime& first, const DateTime::Hours& hours);
    friend DateTime operator-(const DateTime& first, const DateTime::Minutes& mins);
    friend DateTime operator-(const DateTime& first, const DateTime::Seconds& secs);
    friend bool operator==(const DateTime& first, const DateTime& second);
    friend bool operator!=(const DateTime& first, const DateTime& second);
    friend bool operator>=(const DateTime& first, const DateTime& second);
    friend bool operator<=(const DateTime& first, const DateTime& second);
    friend bool operator>(const DateTime& first, const DateTime& second);
    friend bool operator<(const DateTime& first, const DateTime& second);

private:
    boost::posix_time::ptime m_ptime;
};

DateTime operator+(const DateTime& first, const DateTime::Hours& hours);
DateTime operator+(const DateTime& first, const DateTime::Minutes& mins);
DateTime operator+(const DateTime& first, const DateTime::Seconds& secs);
DateTime::Time operator-(const DateTime& first, const DateTime& second);
DateTime operator-(const DateTime& first, const DateTime::Hours& hours);
DateTime operator-(const DateTime& first, const DateTime::Minutes& mins);
DateTime operator-(const DateTime& first, const DateTime::Seconds& secs);
bool operator==(const DateTime& first, const DateTime& second);
bool operator!=(const DateTime& first, const DateTime& second);
bool operator>=(const DateTime& first, const DateTime& second);
bool operator<=(const DateTime& first, const DateTime& second);
bool operator>(const DateTime& first, const DateTime& second);
bool operator<(const DateTime& first, const DateTime& second);
