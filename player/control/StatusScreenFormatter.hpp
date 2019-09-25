#pragma once

#include "GeneralInfo.hpp"
#include "managers/CmsStatus.hpp"
#include "managers/XmrStatus.hpp"
#include "schedule/SchedulerStatus.hpp"

struct StatusInfo
{
    GeneralInfo general;
    CmsStatus cms;
    SchedulerStatus scheduler;
    XmrStatus xmr;
};

class StatusScreenFormatter
{
public:
    static std::string formatInfo(const StatusInfo& info);

private:
    static std::string formatGeneralInfo(const GeneralInfo& info);
    static std::string formatCmsInfo(const CmsStatus& info);
    static std::string formatSchedulerInfo(const SchedulerStatus& info);
    static std::string layoutsToString(const std::vector<int>& layouts);
    static std::string formatXmrInfo(const XmrStatus& info);
};
