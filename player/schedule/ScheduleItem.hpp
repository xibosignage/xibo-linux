#pragma once

#include <vector>
#include <string>
#include "common/DateTime.hpp"

const int EmptyLayoutId = 0;

struct ScheduledLayout;

using LayoutDependants = std::vector<std::string>;
using LayoutList = std::vector<ScheduledLayout>;

struct DefaultScheduledLayout
{
    int id;
    LayoutDependants dependants;
};

struct ScheduledLayout
{
    int scheduleId;
    int id;
    int priority;
    DateTime startDT;
    DateTime endDT;
    LayoutDependants dependants;
};

bool operator== (const DefaultScheduledLayout& first, const DefaultScheduledLayout& second);
bool operator!= (const DefaultScheduledLayout& first, const DefaultScheduledLayout& second);

bool operator== (const ScheduledLayout& first, const ScheduledLayout& second);
bool operator!= (const ScheduledLayout& first, const ScheduledLayout& second);
