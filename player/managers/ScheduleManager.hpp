#pragma once

#include "LayoutSchedule.hpp"
#include "common/FilePath.hpp"
#include "constants.hpp"

class ScheduleManager
{
public:
    void load(const FilePath& path);
    void update(const std::string& schedule);
    LayoutSchedule schedule() const;

private:
    LayoutSchedule parseSchedule(const xml_node& scheduleXml);

    ScheduledLayout parseScheduledLayout(const xml_node& layoutNode);
    DefaultScheduledLayout parseDefaultLayout(const xml_node& layoutNode);
    std::vector<std::string> parseDependants(const xml_node& dependantsNode);

private:
    LayoutSchedule m_schedule;
    FilePath m_path;

};
