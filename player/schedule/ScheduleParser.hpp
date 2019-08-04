#pragma once

#include "constants.hpp"
#include "LayoutSchedule.hpp"

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
    LayoutSchedule scheduleFromImpl(const ptree_node& scheduleXml);
    ScheduledLayout scheduledLayoutFrom(const ptree_node& layoutNode);
    DefaultScheduledLayout defaultLayoutFrom(const ptree_node& layoutNode);
    LayoutList overlayLayoutsFrom(const ptree_node& overlaysNode);
    LayoutDependants dependantsFrom(const ptree_node& dependantsNode);

};
