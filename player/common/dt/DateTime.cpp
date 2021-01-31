#include "DateTime.hpp"

#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "date/tz.h"

DateTime::DateTime(const DateTime::Date& date, const DateTime::Time& td) : ptime_{date, td} {}

DateTime::DateTime(const boost::posix_time::ptime& ptime) : ptime_(ptime) {}

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
    return DateTime{c_local_adjustor<ptime>::utc_to_local(utc.ptime_)};
}

DateTime DateTime::utcFromTimestamp(const time_t& timestamp)
{
    return DateTime{boost::posix_time::from_time_t(timestamp)};
}

std::string DateTime::string(const char* format) const
{
    std::stringstream stream;

    auto facet = new boost::posix_time::time_facet();
    facet->format(format);

    stream.imbue(std::locale(std::locale::classic(), facet));
    stream << ptime_;

    return stream.str();
}

std::string DateTime::string() const
{
    return boost::posix_time::to_simple_string(ptime_);
}

time_t DateTime::timestamp() const
{
    return boost::posix_time::to_time_t(ptime_);
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

std::string DateTime::currentTimezone()
{
    return date::current_zone()->name();
}

bool DateTime::valid() const
{
    return !ptime_.is_not_a_date_time();
}

bool operator==(const DateTime& first, const DateTime& second)
{
    return first.ptime_ == second.ptime_;
}

bool operator!=(const DateTime& first, const DateTime& second)
{
    return first.ptime_ != second.ptime_;
}

bool operator>=(const DateTime& first, const DateTime& second)
{
    return first.ptime_ >= second.ptime_;
}

bool operator<=(const DateTime& first, const DateTime& second)
{
    return first.ptime_ <= second.ptime_;
}

bool operator>(const DateTime& first, const DateTime& second)
{
    return first.ptime_ > second.ptime_;
}

bool operator<(const DateTime& first, const DateTime& second)
{
    return first.ptime_ < second.ptime_;
}

DateTime operator+(const DateTime& first, const DateTime::Hours& hours)
{
    return DateTime{first.ptime_ + hours};
}

DateTime operator+(const DateTime& first, const DateTime::Minutes& mins)
{
    return DateTime{first.ptime_ + mins};
}

DateTime operator+(const DateTime& first, const DateTime::Seconds& secs)
{
    return DateTime{first.ptime_ + secs};
}

DateTime::Time operator-(const DateTime& first, const DateTime& second)
{
    return first.ptime_ - second.ptime_;
}

DateTime operator-(const DateTime& first, const DateTime::Hours& hours)
{
    return DateTime{first.ptime_ - hours};
}

DateTime operator-(const DateTime& first, const DateTime::Minutes& mins)
{
    return DateTime{first.ptime_ - mins};
}

DateTime operator-(const DateTime& first, const DateTime::Seconds& secs)
{
    return DateTime{first.ptime_ - secs};
}
