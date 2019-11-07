#pragma once

#include "common/dt/DateTime.hpp"

struct XmrStatus
{
    // TODO: strong type
    std::string host;
    DateTime lastMessageDt;
    DateTime lastHeartbeatDt;
};
