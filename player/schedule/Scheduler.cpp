#include "Scheduler.hpp"

#include "common/logger/Logging.hpp"
#include "common/dt/DateTimeProvider.hpp"
#include "common/fs/FileCache.hpp"
#include "common/fs/Resources.hpp"

Scheduler::Scheduler(const FileCache& fileCache) :
    m_fileCache(fileCache)
{
}

void Scheduler::reloadSchedule(LayoutSchedule&& schedule)
{
    if(m_schedule != schedule)
    {
        m_schedule = std::move(schedule);
        m_scheduleUpdated.emit(m_schedule);
    }
}

int Scheduler::nextLayoutId()
{
    m_currentLayoutId = nextScheduledLayoutId();

    auto&& defaultLayout = m_schedule.defaultLayout;
    if(m_currentLayoutId == EmptyLayoutId && isLayoutValid(defaultLayout.dependants))
    {
        m_currentLayoutId = defaultLayout.id;
    }

    return m_currentLayoutId;
}

std::vector<int> Scheduler::nextOverlayLayoutsIds()
{
    std::vector<int> overlayLayoutsIds;

    for(auto&& layout : m_schedule.overlayLayouts)
    {
        if(isLayoutOnSchedule(layout) && isLayoutValid(layout.dependants))
        {
            overlayLayoutsIds.emplace_back(layout.id);
        }
    }

    return overlayLayoutsIds;
}

int Scheduler::currentLayoutId() const
{
    return m_currentLayoutId;
}

SignalScheduleAvailable Scheduler::scheduleUpdated()
{
    return m_scheduleUpdated;
}

SchedulerStatus Scheduler::status()
{
    SchedulerStatus status;

    collectLayoutListStatus(status, m_schedule.regularLayouts);
    collectLayoutListStatus(status, m_schedule.overlayLayouts);

    status.generatedTime = DateTimeProvider::toString(m_schedule.generatedTime);
    status.currentLayout = m_currentLayoutId;

    return status;
}

template<typename LayoutsList>
void Scheduler::collectLayoutListStatus(SchedulerStatus& status, const LayoutsList& layouts)
{
    for(auto&& layout : layouts)
    {
        if(isLayoutValid(layout.dependants))
        {
            status.validLayouts.emplace_back(layout.id);
            if(isLayoutOnSchedule(layout))
            {
                status.scheduledLayouts.emplace_back(layout.id);
            }
        }
        else
        {
            status.invalidLayouts.emplace_back(layout.id);
        }
    }
}

int Scheduler::nextScheduledLayoutId()
{
    auto&& regularLayouts = m_schedule.regularLayouts;
    for(size_t i = 0; i != regularLayouts.size(); ++i)
    {
        auto&& layout = regularLayouts[i];

        if(isLayoutOnSchedule(layout) && isLayoutValid(layout.dependants))
        {
            return layout.id;
        }
    }

    return EmptyLayoutId;
}

bool Scheduler::isLayoutOnSchedule(const ScheduledLayout& layout) const
{
    auto currentDT = DateTimeProvider::now();

    if(currentDT >= layout.startDT && currentDT < layout.endDT)
    {
        return true;
    }

    return false;
}

bool Scheduler::isLayoutValid(const std::vector<std::string>& dependants) const
{
    for(auto&& dependant : m_schedule.globalDependants)
    {
        if(m_fileCache.cached(dependant))
        {
            return false;
        }
    }

    for(auto&& dependant : dependants)
    {
        if(m_fileCache.cached(dependant))
        {
            return false;
        }
    }

    return true;
}
