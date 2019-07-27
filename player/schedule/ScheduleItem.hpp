#pragma once

#include <vector>
#include <string>
#include "common/DateTime.hpp"

const int EmptyLayoutId = 0;

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
    DateTime startDT;
    DateTime endDT;
    std::vector<std::string> dependants;
};
