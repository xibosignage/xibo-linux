#pragma once

#include "ScheduleItem.hpp"

struct LayoutSchedule
{
    std::vector<std::string> globalDependants;
    std::vector<ScheduledLayout> layouts;
    DefaultScheduledLayout defaultLayout;
};
