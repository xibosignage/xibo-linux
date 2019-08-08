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
    LayoutId current = currentLayoutId();
    OverlaysIds overlays = overlayLayouts();

    m_regularQueue = regularQueueFrom(m_schedule);
    m_overlayQueue = overlayQueueFrom(m_schedule);

    updateCurrentLayout(current);
    updateCurrentOverlays(overlays);
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

void Scheduler::updateCurrentLayout(LayoutId id)
{
    if(m_regularQueue.inQueue(id))
    {
        m_regularQueue.updateCurrent(id);
    }
    else
    {
        m_layoutUpdated.emit();
    }
}

void Scheduler::updateCurrentOverlays(const OverlaysIds& ids)
{
    if(m_overlayQueue.overlays() != ids)
    {
        m_overlaysUpdated.emit();
    }
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

LayoutId Scheduler::nextLayout() const
{
    return m_regularQueue.next();
}

LayoutId Scheduler::currentLayoutId() const
{
    return m_regularQueue.current();
}

OverlaysIds Scheduler::overlayLayouts() const
{
    return m_overlayQueue.overlays();
}

SignalScheduleAvailable Scheduler::scheduleUpdated()
{
    return m_scheduleUpdated;
}

SignalLayoutsUpdated Scheduler::overlaysUpdated()
{
    return m_overlaysUpdated;
}

SignalLayoutsUpdated Scheduler::layoutUpdated()
{
    return m_layoutUpdated;
}

SchedulerStatus Scheduler::status() const
{
    SchedulerStatus status;

    schedulerStatus(status, m_schedule.regularLayouts);
    schedulerStatus(status, m_schedule.overlayLayouts);

    status.generatedTime = DateTimeProvider::toString(m_schedule.generatedTime);
    status.currentLayout = currentLayoutId();

    return status;
}

template<typename LayoutsList>
void Scheduler::schedulerStatus(SchedulerStatus& status, const LayoutsList& layouts) const
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
