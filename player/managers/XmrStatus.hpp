#pragma once

#include <boost/date_time/posix_time/ptime.hpp>

struct XmrStatus
{
    std::string host;
    boost::posix_time::ptime lastMessageDt;
    boost::posix_time::ptime lastHeartbeatDt;
};
