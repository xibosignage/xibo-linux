#pragma once

#include <string>
#include <vector>

class StatusInfo;
class GeneralInfo;
class SchedulerStatus;
class XmrStatus;
class CmsStatus;

class StatusScreenFormatter
{
public:
    std::string formatInfo(const StatusInfo& info);

private:
    std::string formatGeneralInfo(const GeneralInfo& info);
    std::string formatCmsInfo(const CmsStatus& info);
    std::string formatSchedulerInfo(const SchedulerStatus& info);
    std::string layoutsToString(const std::vector<int>& layouts);
    std::string formatXmrInfo(const XmrStatus& info);
};
