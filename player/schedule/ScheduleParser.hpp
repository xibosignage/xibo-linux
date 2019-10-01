#pragma once

#include "constants.hpp"
#include "schedule/LayoutSchedule.hpp"

class FilePath;

class ScheduleParser
{
public:
    struct Error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    LayoutSchedule scheduleFrom(const FilePath& path);
    LayoutSchedule scheduleFrom(const std::string& xmlSchedule);

private:
    LayoutSchedule scheduleFromImpl(const PtreeNode& scheduleXml);
    ScheduledLayout scheduledLayoutFrom(const PtreeNode& layoutNode);
    DefaultScheduledLayout defaultLayoutFrom(const PtreeNode& layoutNode);
    LayoutList overlayLayoutsFrom(const PtreeNode& overlaysNode);
    LayoutDependants dependantsFrom(const PtreeNode& dependantsNode);
};
