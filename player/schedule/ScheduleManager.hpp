#pragma once

#include "common/FilePath.hpp"
#include "ParsedLayoutSchedule.hpp"

class ScheduleSerializer;

class ScheduleManager
{
public:
    ScheduleManager(std::unique_ptr<ScheduleSerializer>&& serializer);

    void load(const FilePath& path);
    void update(const std::string& schedule);
    ParsedLayoutSchedule schedule() const;

private:
    std::unique_ptr<ScheduleSerializer> m_serializer;
    ParsedLayoutSchedule m_cachedSchedule;
    FilePath m_path;

};
