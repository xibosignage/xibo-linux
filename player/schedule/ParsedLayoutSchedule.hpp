#pragma once

#include "ScheduleItem.hpp"

struct ParsedLayoutSchedule
{
    std::string generatedTime;
    std::vector<std::string> globalDependants;
    std::vector<ScheduledLayout> regularLayouts;
    std::vector<ScheduledLayout> overlayLayouts;
    DefaultScheduledLayout defaultLayout;
};
