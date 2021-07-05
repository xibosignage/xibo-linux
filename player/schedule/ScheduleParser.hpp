#pragma once

#include "common/parsing/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"
#include "schedule/LayoutSchedule.hpp"

class FilePath;

class ScheduleParser
{
public:
    DECLARE_EXCEPTION(ScheduleParser)

    LayoutSchedule scheduleFrom(const FilePath& path);
    LayoutSchedule scheduleFrom(const std::string& xmlSchedule);

private:
    LayoutSchedule scheduleFromImpl(const XmlNode& scheduleXml);
    ScheduledLayout scheduledLayoutFrom(const XmlNode& layoutNode);
    DefaultScheduledLayout defaultLayoutFrom(const XmlNode& layoutNode);
    LayoutList overlayLayoutsFrom(const XmlNode& overlaysNode);
    ScheduledCommand commandFrom(const XmlNode& overlaysNode);
    LayoutDependants dependantsFrom(const XmlNode& dependantsNode);
};
