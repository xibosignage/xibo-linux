#pragma once

#include "control/GeneralInfo.hpp"
#include "control/StatusScreenFormatter.hpp"
#include "managers/CmsStatus.hpp"
#include "managers/XmrStatus.hpp"
#include "schedule/SchedulerStatus.hpp"

struct StatusInfo
{
    std::string toString() const
    {
        StatusScreenFormatter formatter;
        return formatter.formatInfo(*this);
    }

    GeneralInfo general;
    CmsStatus cms;
    SchedulerStatus scheduler;
    XmrStatus xmr;
};
