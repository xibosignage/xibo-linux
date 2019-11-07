#pragma once

#include "control/GeneralInfo.hpp"
#include "control/StatusScreenFormatter.hpp"
#include "managers/CmsStatus.hpp"
#include "schedule/SchedulerStatus.hpp"
#include "xmr/XmrStatus.hpp"

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
