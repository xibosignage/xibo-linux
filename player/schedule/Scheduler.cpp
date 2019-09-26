#include "Scheduler.hpp"

#include "common/dt/DateTime.hpp"
#include "common/fs/Resources.hpp"
#include "common/logger/Logging.hpp"

Scheduler::Scheduler(const IFileCache& fileCache) : m_fileCache{fileCache}, m_schedule{} {}

// TODO make an optional to differentiate between empty and non-inialized schedule
void Scheduler::reloadSchedule(LayoutSchedule&& schedule)
{
    if (m_schedule != schedule)
    {
        m_schedule = std::move(schedule);
        m_scheduleUpdated(m_schedule);

        reloadQueue();
    }
}

void Scheduler::reloadQueue()
{
    auto current = currentLayoutId();
    auto overlays = overlayLayouts();

    m_regularQueue = regularQueueFrom(m_schedule);
    m_overlayQueue = overlayQueueFrom(m_schedule);
    restartTimer();

    updateCurrentLayout(current);
    updateCurrentOverlays(overlays);
}

RegularLayoutQueue Scheduler::regularQueueFrom(const LayoutSchedule& schedule)
{
    RegularLayoutQueue queue;

    for (auto&& layout : schedule.regularLayouts)
    {
        if (layoutOnSchedule(layout) && layoutValid(layout))
        {
            queue.add(layout);
        }
    }

    if (layoutValid(schedule.defaultLayout))
    {
        queue.addDefault(schedule.defaultLayout);
    }

    return queue;
}

OverlayLayoutQueue Scheduler::overlayQueueFrom(const LayoutSchedule& schedule)
{
    OverlayLayoutQueue queue;

    for (auto&& layout : schedule.overlayLayouts)
    {
        if (layoutOnSchedule(layout) && layoutValid(layout))
        {
            queue.add(layout);
        }
    }

    return queue;
}

void Scheduler::updateCurrentLayout(LayoutId id)
{
    if (m_regularQueue.inQueue(id))
    {
        m_regularQueue.updateCurrent(id);
    }
    else
    {
        m_layoutUpdated();
    }
}

void Scheduler::updateCurrentOverlays(const OverlaysIds& ids)
{
    if (m_overlayQueue.overlays() != ids)
    {
        m_overlaysUpdated();
    }
}

bool Scheduler::layoutOnSchedule(const ScheduledLayout& layout) const
{
    auto currentDT = DateTime::now();

    if (currentDT >= layout.startDT && currentDT < layout.endDT)
    {
        return true;
    }

    return false;
}

template <typename Layout>
bool Scheduler::layoutValid(const Layout& layout) const
{
    auto layoutFile = std::to_string(layout.id) + ".xlf";
    if (!m_fileCache.valid(layoutFile)) return false;

    for (auto&& dependant : layout.dependants)
    {
        if (!m_fileCache.valid(dependant))
        {
            return false;
        }
    }

    for (auto&& dependant : m_schedule.globalDependants)
    {
        if (!m_fileCache.valid(dependant))
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

SignalScheduleUpdated& Scheduler::scheduleUpdated()
{
    return m_scheduleUpdated;
}

SignalLayoutsUpdated& Scheduler::overlaysUpdated()
{
    return m_overlaysUpdated;
}

DateTime Scheduler::closestLayoutDt()
{
    auto now = DateTime::now();
    DateTime closestDt;

    for (auto&& layout : m_schedule.regularLayouts)
    {
        if (now < layout.startDT && layout.startDT < closestDt) closestDt = layout.startDT;
        if (now < layout.endDT && layout.endDT < closestDt) closestDt = layout.endDT;
    }

    for (auto&& layout : m_schedule.overlayLayouts)
    {
        if (now < layout.startDT && layout.startDT < closestDt) closestDt = layout.startDT;
        if (now < layout.endDT && layout.endDT < closestDt) closestDt = layout.endDT;
    }

    return closestDt;
}

void Scheduler::restartTimer()
{
    auto dt = closestLayoutDt();
    auto duration = (dt - DateTime::now()).total_seconds();

    if (dt.isValid() && duration > 0)
    {
        Log::debug("Timer restarted with value: {}", duration);

        m_timer.start(std::chrono::seconds(duration), std::bind(&Scheduler::reloadQueue, this));
    }
    else
    {
        Log::debug("No timer detected");
    }
}

SignalLayoutsUpdated& Scheduler::layoutUpdated()
{
    return m_layoutUpdated;
}

SchedulerStatus Scheduler::status() const
{
    SchedulerStatus status;

    schedulerStatus(status, m_schedule.regularLayouts);
    schedulerStatus(status, m_schedule.overlayLayouts);

    status.generatedTime = DateTime::toString(m_schedule.generatedTime);
    status.currentLayout = currentLayoutId();

    return status;
}

template <typename LayoutsList>
void Scheduler::schedulerStatus(SchedulerStatus& status, const LayoutsList& layouts) const
{
    for (auto&& layout : layouts)
    {
        if (layoutValid(layout))
        {
            status.validLayouts.emplace_back(layout.id);
            if (layoutOnSchedule(layout))
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
