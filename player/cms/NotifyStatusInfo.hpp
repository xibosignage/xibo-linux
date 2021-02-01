#pragma once

#include "common/fs/StorageUsageInfo.hpp"
#include "common/system/Hostname.hpp"
#include "schedule/ScheduleItem.hpp"

struct NotifyStatusInfo
{
    std::string string() const;

    LayoutId currentLayoutId;
    StorageUsageInfo spaceUsageInfo;
    Hostname deviceName;
    std::string timezone;
};
