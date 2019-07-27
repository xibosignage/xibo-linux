#pragma once

#include "IScheduleSerializer.hpp"
#include "constants.hpp"

struct ScheduleParseException : public std::exception
{
    const char* what() const noexcept override;
};

class ScheduleSerializer : public IScheduleSerializer
{
public:
    LayoutSchedule parseSchedule(const FilePath& path) override;
    LayoutSchedule parseSchedule(const std::string& xmlSchedule) override;

private:
    LayoutSchedule parseScheduleImpl(const xml_node& scheduleXml);
    ScheduledLayout parseScheduledLayout(const xml_node& layoutNode);
    DefaultScheduledLayout parseDefaultLayout(const xml_node& layoutNode);
    std::vector<ScheduledLayout> parseOverlayLayouts(const xml_node& overlaysNode);
    std::vector<std::string> parseDependants(const xml_node& dependantsNode);

};
