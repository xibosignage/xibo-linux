#pragma once

#include "common/fs/StorageUsageInfo.hpp"
#include "schedule/ScheduleItem.hpp"

struct NotifyStatusInfo
{
    std::string string() const;

    LayoutId currentLayoutId;
    StorageUsageInfo spaceUsageInfo;
    std::string deviceName;
    std::string timezone;
};
