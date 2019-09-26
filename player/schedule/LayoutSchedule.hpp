#pragma once

#include "ScheduleItem.hpp"

struct LayoutSchedule
{
    DateTime generatedTime;
    LayoutDependants globalDependants;
    LayoutList regularLayouts;
    LayoutList overlayLayouts;
    DefaultScheduledLayout defaultLayout;
};

bool operator==(const LayoutSchedule& first, const LayoutSchedule& second);
bool operator!=(const LayoutSchedule& first, const LayoutSchedule& second);
