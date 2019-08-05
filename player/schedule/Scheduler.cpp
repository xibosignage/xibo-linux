#include "Scheduler.hpp"

#include "common/logger/Logging.hpp"
#include "common/dt/DateTimeProvider.hpp"
#include "common/fs/Resources.hpp"

Scheduler::Scheduler(const IFileCache& fileCache) :
    m_fileCache{fileCache},
    m_schedule{}
{
}

void Scheduler::reloadSchedule(LayoutSchedule&& schedule)
{
    if(m_schedule != schedule)
    {
        m_schedule = std::move(schedule);
        m_scheduleUpdated.emit(m_schedule);

        reloadQueue();
    }
}

void Scheduler::reloadQueue()
{
    m_regularQueue = regularQueueFrom(m_schedule);
    m_overlayQueue = overlayQueueFrom(m_schedule);

    updateInQueue(currentLayoutId());
}

void Scheduler::updateInQueue(int id)
{
    if(m_regularQueue.inQueue(id))
    {
        m_regularQueue.setCurrentLayout(id);
    }
    else
    {
        m_layoutInterrupted.emit();
    }
}

RegularLayoutQueue Scheduler::regularQueueFrom(const LayoutSchedule& schedule)
{
    RegularLayoutQueue queue;

    for(auto&& layout : schedule.regularLayouts)
    {        
        if(layoutOnSchedule(layout) && layoutValid(layout))
        {
            queue.add(layout);
        }
    }

    if(layoutValid(schedule.defaultLayout))
    {
        queue.addDefault(schedule.defaultLayout);
    }

    return queue;
}

OverlayLayoutQueue Scheduler::overlayQueueFrom(const LayoutSchedule& schedule)
{
    OverlayLayoutQueue queue;

    for(auto&& layout : schedule.overlayLayouts)
    {
        if(layoutOnSchedule(layout) && layoutValid(layout))
        {
            queue.add(layout);
        }
    }

    return queue;
}

bool Scheduler::layoutOnSchedule(const ScheduledLayout& layout) const
{
    auto currentDT = DateTimeProvider::now();

    if(currentDT >= layout.startDT && currentDT < layout.endDT)
    {
        return true;
    }

    return false;
}

template<typename Layout>
bool Scheduler::layoutValid(const Layout& layout) const
{
    auto layoutFile = std::to_string(layout.id) + ".xlf";
    if(!m_fileCache.cached(layoutFile)) return false;

    for(auto&& dependant : layout.dependants)
    {
        if(!m_fileCache.cached(dependant))
        {
            return false;
        }
    }

    for(auto&& dependant : m_schedule.globalDependants)
    {
        if(!m_fileCache.cached(dependant))
        {
            return false;
        }
    }

    return true;
}

int Scheduler::nextLayout()
{
    m_currentLayoutId = m_regularQueue.next();

    return m_currentLayoutId;
}

std::vector<int> Scheduler::nextOverlayLayouts()
{
    return m_overlayQueue.next();
}

int Scheduler::currentLayoutId() const
{
    return m_currentLayoutId;
}

SignalScheduleAvailable Scheduler::scheduleUpdated()
{
    return m_scheduleUpdated;
}

SignalLayoutInterrupted Scheduler::layoutInterrupted()
{
    return m_layoutInterrupted;
}

SchedulerStatus Scheduler::status()
{
    SchedulerStatus status;

    schedulerStatus(status, m_schedule.regularLayouts);
    schedulerStatus(status, m_schedule.overlayLayouts);

    status.generatedTime = DateTimeProvider::toString(m_schedule.generatedTime);
    status.currentLayout = m_currentLayoutId;

    return status;
}

template<typename LayoutsList>
void Scheduler::schedulerStatus(SchedulerStatus& status, const LayoutsList& layouts)
{
    for(auto&& layout : layouts)
    {
        if(layoutValid(layout))
        {
            status.validLayouts.emplace_back(layout.id);
            if(layoutOnSchedule(layout))
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
