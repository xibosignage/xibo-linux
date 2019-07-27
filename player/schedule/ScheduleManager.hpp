#pragma once

#include "common/fs/FilePath.hpp"
#include "ParsedLayoutSchedule.hpp"
#include "IScheduleSerializer.hpp"

class ScheduleManager
{
public:
    ScheduleManager(std::unique_ptr<IScheduleSerializer>&& serializer);

    void load(const FilePath& path);
    void update(const std::string& schedule);
    LayoutSchedule schedule() const;

private:
    void writeToFile(const std::string& scheduleXml);

private:
    std::unique_ptr<IScheduleSerializer> m_serializer;
    LayoutSchedule m_cachedSchedule;
    FilePath m_path;

};
