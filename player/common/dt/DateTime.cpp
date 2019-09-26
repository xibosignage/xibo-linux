#include "DateTime.hpp"

#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

DateTime::DateTime(const DateTime::Date& date, const DateTime::Time& td) : m_ptime{date, td} {}

DateTime::DateTime(const boost::posix_time::ptime& ptime) : m_ptime(ptime) {}

DateTime DateTime::now()
{
    return DateTime{boost::posix_time::second_clock::local_time()};
}

DateTime DateTime::nowUtc()
{
    return DateTime{boost::posix_time::second_clock::universal_time()};
}

DateTime DateTime::localFromTimestamp(const time_t& timestamp)
{
    using boost::date_time::c_local_adjustor;
    using boost::posix_time::from_time_t;
    using boost::posix_time::ptime;
    auto utc = utcFromTimestamp(timestamp);
    return DateTime{c_local_adjustor<ptime>::utc_to_local(utc.m_ptime)};
}

DateTime DateTime::utcFromTimestamp(const time_t& timestamp)
{
    return DateTime{boost::posix_time::from_time_t(timestamp)};
}

std::string DateTime::toString(const DateTime& dt, const char* format)
{
    std::stringstream stream;

    auto facet = new boost::posix_time::time_facet();
    facet->format(format);

    stream.imbue(std::locale(std::locale::classic(), facet));
    stream << dt.m_ptime;

    return stream.str();
}

std::string DateTime::toString(const DateTime& dt)
{
    return boost::posix_time::to_simple_string(dt.m_ptime);
}

DateTime DateTime::fromString(const std::string& str)
{
    return DateTime{boost::posix_time::time_from_string(str)};
}

DateTime DateTime::fromIsoExtendedString(const std::string& str)
{
    auto dtFacet = new boost::posix_time::time_input_facet;
    dtFacet->set_iso_extended_format();

    boost::posix_time::ptime dt;
    std::istringstream iss{str};
    iss >> dt;

    return DateTime{dt};
}

bool DateTime::isValid() const
{
    return !m_ptime.is_not_a_date_time();
}

bool operator==(const DateTime& first, const DateTime& second)
{
    return first.m_ptime == second.m_ptime;
}

bool operator!=(const DateTime& first, const DateTime& second)
{
    return first.m_ptime != second.m_ptime;
}

bool operator>=(const DateTime& first, const DateTime& second)
{
    return first.m_ptime >= second.m_ptime;
}

bool operator<=(const DateTime& first, const DateTime& second)
{
    return first.m_ptime <= second.m_ptime;
}

bool operator>(const DateTime& first, const DateTime& second)
{
    return first.m_ptime > second.m_ptime;
}

bool operator<(const DateTime& first, const DateTime& second)
{
    return first.m_ptime < second.m_ptime;
}

DateTime operator+(const DateTime& first, const DateTime::Hours& hours)
{
    return DateTime{first.m_ptime + hours};
}

DateTime operator+(const DateTime& first, const DateTime::Minutes& mins)
{
    return DateTime{first.m_ptime + mins};
}

DateTime operator+(const DateTime& first, const DateTime::Seconds& secs)
{
    return DateTime{first.m_ptime + secs};
}

DateTime::Time operator-(const DateTime& first, const DateTime& second)
{
    return first.m_ptime - second.m_ptime;
}

DateTime operator-(const DateTime& first, const DateTime::Hours& hours)
{
    return DateTime{first.m_ptime - hours};
}

DateTime operator-(const DateTime& first, const DateTime::Minutes& mins)
{
    return DateTime{first.m_ptime - mins};
}

DateTime operator-(const DateTime& first, const DateTime::Seconds& secs)
{
    return DateTime{first.m_ptime - secs};
}
