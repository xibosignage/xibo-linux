#pragma once

#include "ScheduleItem.hpp"

struct LayoutSchedule
{
    std::string generatedTime;
    std::vector<std::string> globalDependants;
    std::vector<ScheduledLayout> regularLayouts;
    std::vector<ScheduledLayout> overlayLayouts;
    DefaultScheduledLayout defaultLayout;
};

bool operator== (const LayoutSchedule& first, const LayoutSchedule& second);
bool operator!= (const LayoutSchedule& first, const LayoutSchedule& second);
