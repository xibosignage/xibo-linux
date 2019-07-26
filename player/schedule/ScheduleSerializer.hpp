#pragma once

#include "ParsedLayoutSchedule.hpp"
#include "constants.hpp"

class FilePath;

struct ScheduleParseException : public std::exception
{
    const char* what() const noexcept override;
};

class ScheduleSerializer
{
public:
    ParsedLayoutSchedule parseSchedule(const FilePath& path);
    ParsedLayoutSchedule parseSchedule(const std::string& xmlSchedule);

private:
    ParsedLayoutSchedule parseScheduleImpl(const xml_node& scheduleXml);
    ScheduledLayout parseScheduledLayout(const xml_node& layoutNode);
    DefaultScheduledLayout parseDefaultLayout(const xml_node& layoutNode);
    std::vector<ScheduledLayout> parseOverlayLayouts(const xml_node& overlaysNode);
    std::vector<std::string> parseDependants(const xml_node& dependantsNode);

};
