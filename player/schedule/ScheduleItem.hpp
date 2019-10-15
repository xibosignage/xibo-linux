#pragma once

#include <string>
#include <vector>

#include "common/dt/DateTime.hpp"

struct ScheduledLayout;
using LayoutDependants = std::vector<std::string>;
using LayoutList = std::vector<ScheduledLayout>;
using LayoutId = int;

const LayoutId EmptyLayoutId = 0;
const int DefaultScheduleId = 0;

struct DefaultScheduledLayout
{
    LayoutId id;
    LayoutDependants dependants;
};

struct ScheduledLayout
{
    int scheduleId;
    LayoutId id;
    int priority;
    DateTime startDT;
    DateTime endDT;
    LayoutDependants dependants;
};

bool operator==(const DefaultScheduledLayout& first, const DefaultScheduledLayout& second);
bool operator!=(const DefaultScheduledLayout& first, const DefaultScheduledLayout& second);

bool operator==(const ScheduledLayout& first, const ScheduledLayout& second);
bool operator!=(const ScheduledLayout& first, const ScheduledLayout& second);
