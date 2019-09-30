#pragma once

#include "LayoutSchedule.hpp"
#include "constants.hpp"

class FilePath;

struct ScheduleParseException : public std::exception
{
    const char* what() const noexcept override;
};

class ScheduleParser
{
public:
    LayoutSchedule scheduleFrom(const FilePath& path);
    LayoutSchedule scheduleFrom(const std::string& xmlSchedule);

private:
    LayoutSchedule scheduleFromImpl(const PtreeNode& scheduleXml);
    ScheduledLayout scheduledLayoutFrom(const PtreeNode& layoutNode);
    DefaultScheduledLayout defaultLayoutFrom(const PtreeNode& layoutNode);
    LayoutList overlayLayoutsFrom(const PtreeNode& overlaysNode);
    LayoutDependants dependantsFrom(const PtreeNode& dependantsNode);
};
