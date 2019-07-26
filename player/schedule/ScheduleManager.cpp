#include "ScheduleManager.hpp"
#include "ScheduleSerializer.hpp"

#include "common/FileSystem.hpp"

#include <fstream>

ScheduleManager::ScheduleManager(std::unique_ptr<ScheduleSerializer>&& serializer) :
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
    m_cachedSchedule = m_serializer->parseSchedule(scheduleXml);

    std::ofstream out(m_path.string());
    out << scheduleXml;
}

ParsedLayoutSchedule ScheduleManager::schedule() const
{
    return m_cachedSchedule;
}
