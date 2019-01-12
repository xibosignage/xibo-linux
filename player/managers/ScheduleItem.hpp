#pragma once

#include <vector>
#include <string>

struct DefaultScheduledLayout
{
    int id;
    std::vector<std::string> dependants;
};

struct ScheduledLayout
{
    int scheduleId;
    int id;
    int priority;
    std::string startDT;
    std::string endDT;
    std::vector<std::string> dependants;
};
