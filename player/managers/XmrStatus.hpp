#pragma once

#include "common/DateTime.hpp"

struct XmrStatus
{
    std::string host;
    DateTime lastMessageDt;
    DateTime lastHeartbeatDt;
};
