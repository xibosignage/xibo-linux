#include "Scheduler.hpp"

#include "common/dt/DateTime.hpp"
#include "common/fs/Resources.hpp"
#include "common/logger/Logging.hpp"

Scheduler::Scheduler(const IFileCache& fileCache) : fileCache_{fileCache}, schedule_{} {}

// TODO make an optional to differentiate between empty and non-inialized schedule
void Scheduler::reloadSchedule(LayoutSchedule&& schedule)
{
    if (schedule_ != schedule)
    {
        schedule_ = std::move(schedule);
        scheduleUpdated_(schedule_);

        reloadQueue();
    }
}

void Scheduler::reloadQueue()
{
    auto current = currentLayoutId();
    auto overlays = overlayLayouts();

    regularQueue_ = regularQueueFrom(schedule_);
    overlayQueue_ = overlayQueueFrom(schedule_);
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
    if (regularQueue_.inQueue(id))
    {
        regularQueue_.updateCurrent(id);
    }
    else
    {
        layoutUpdated_();
    }
}

void Scheduler::updateCurrentOverlays(const OverlaysIds& ids)
{
    if (overlayQueue_.overlays() != ids)
    {
        overlaysUpdated_();
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
    if (!fileCache_.valid(layoutFile)) return false;

    for (auto&& dependant : layout.dependants)
    {
        if (!fileCache_.valid(dependant))
        {
            return false;
        }
    }

    for (auto&& dependant : schedule_.globalDependants)
    {
        if (!fileCache_.valid(dependant))
        {
            return false;
        }
    }

    return true;
}

LayoutId Scheduler::nextLayout() const
{
    return regularQueue_.next();
}

LayoutId Scheduler::currentLayoutId() const
{
    return regularQueue_.current();
}

OverlaysIds Scheduler::overlayLayouts() const
{
    return overlayQueue_.overlays();
}

SignalScheduleUpdated& Scheduler::scheduleUpdated()
{
    return scheduleUpdated_;
}

SignalLayoutsUpdated& Scheduler::overlaysUpdated()
{
    return overlaysUpdated_;
}

DateTime Scheduler::closestLayoutDt()
{
    auto now = DateTime::now();
    DateTime closestDt;

    for (auto&& layout : schedule_.regularLayouts)
    {
        if (now < layout.startDT && layout.startDT < closestDt) closestDt = layout.startDT;
        if (now < layout.endDT && layout.endDT < closestDt) closestDt = layout.endDT;
    }

    for (auto&& layout : schedule_.overlayLayouts)
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

        timer_.start(std::chrono::seconds(duration), std::bind(&Scheduler::reloadQueue, this));
    }
}

SignalLayoutsUpdated& Scheduler::layoutUpdated()
{
    return layoutUpdated_;
}

SchedulerStatus Scheduler::status() const
{
    SchedulerStatus status;

    fillSchedulerStatus(status, schedule_.regularLayouts);
    fillSchedulerStatus(status, schedule_.overlayLayouts);

    status.generatedTime = DateTime::toString(schedule_.generatedTime);
    status.currentLayout = currentLayoutId();

    return status;
}

template <typename LayoutsList>
void Scheduler::fillSchedulerStatus(SchedulerStatus& status, const LayoutsList& layouts) const
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
