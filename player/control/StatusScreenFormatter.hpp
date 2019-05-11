#pragma once

#include "managers/SchedulerStatus.hpp"
#include "GeneralInfo.hpp"
#include "managers/CmsStatus.hpp"

struct StatusInfo
{
    GeneralInfo general;
    CmsStatus cms;
    SchedulerStatus scheduler;
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

};
