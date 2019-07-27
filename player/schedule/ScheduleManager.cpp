#include "ScheduleManager.hpp"
#include "ScheduleSerializer.hpp"

#include "common/fs/FileSystem.hpp"

#include <fstream>

ScheduleManager::ScheduleManager(std::unique_ptr<IScheduleSerializer>&& serializer) :
    m_serializer(std::move(serializer))
{
}

void ScheduleManager::load(const FilePath& path)
{
    if(FileSystem::exists(path))
    {
        m_cachedSchedule = m_serializer->parseSchedule(path);
    }

    m_path = path;
}

void ScheduleManager::update(const std::string& scheduleXml)
{
    auto schedule = m_serializer->parseSchedule(scheduleXml);

    if(schedule != m_cachedSchedule)
    {
        m_cachedSchedule = std::move(schedule);

        FileSystem::writeToFile(m_path, scheduleXml);
    }
}

LayoutSchedule ScheduleManager::schedule() const
{
    return m_cachedSchedule;
}
