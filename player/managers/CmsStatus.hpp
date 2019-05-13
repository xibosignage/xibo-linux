#pragma once

#include <boost/date_time/posix_time/ptime.hpp>

struct CmsStatus
{
    bool registered;
    boost::posix_time::ptime lastChecked;
    size_t requiredFiles;
};
