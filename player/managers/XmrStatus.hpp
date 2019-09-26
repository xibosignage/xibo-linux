#pragma once

#include "common/dt/DateTime.hpp"

struct XmrStatus
{
    std::string host;
    DateTime lastMessageDt;
    DateTime lastHeartbeatDt;
};
