#include "DateTimeProvider.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

DateTime DateTimeProvider::now()
{
    return boost::posix_time::second_clock::local_time();
}

DateTime DateTimeProvider::nowUtc()
{
    return boost::posix_time::second_clock::universal_time();
}

DateTime DateTimeProvider::fromTimeT(const time_t& timestamp)
{
    using boost::date_time::c_local_adjustor;
    using boost::posix_time::from_time_t;
    using boost::posix_time::ptime;
    return c_local_adjustor<ptime>::utc_to_local(utcFromTimeT(timestamp));
}

DateTime DateTimeProvider::utcFromTimeT(const time_t& timestamp)
{
    return boost::posix_time::from_time_t(timestamp);
}

std::string DateTimeProvider::toString(const DateTime& dt, const char* format)
{
    std::stringstream stream;

    auto facet = new boost::posix_time::time_facet();
    facet->format(format);

    stream.imbue(std::locale(std::locale::classic(), facet));
    stream << dt;

    return stream.str();
}

std::string DateTimeProvider::toString(const DateTime& dt)
{
    return boost::posix_time::to_simple_string(dt);
}

DateTime DateTimeProvider::fromString(const std::string& str)
{
    return boost::posix_time::time_from_string(str);
}

DateTime DateTimeProvider::fromIsoExtendedString(const std::string& str)
{
    auto dtFacet = new boost::posix_time::time_input_facet;
    dtFacet->set_iso_extended_format();

    DateTime dt;
    std::istringstream iss{str};
    iss >> dt;

    return dt;
}
