#pragma once

#include "cms/CmsStatus.hpp"
#include "control/status/GeneralInfo.hpp"
#include "control/status/StatusScreenFormatter.hpp"
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
