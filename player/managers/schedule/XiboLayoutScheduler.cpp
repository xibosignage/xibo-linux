#include "XiboLayoutScheduler.hpp"

#include "common/logger/Logging.hpp"
#include "common/FileSystem.hpp"
#include "utils/Resources.hpp"

void XiboLayoutScheduler::reloadSchedule(LayoutSchedule&& schedule)
{
    m_schedule = std::move(schedule);
}

int XiboLayoutScheduler::nextLayoutId()
{
    m_currentLayoutId = nextScheduledLayoutId();

    auto&& defaultLayout = m_schedule.defaultLayout();
    if(m_currentLayoutId == EmptyLayoutId && isLayoutValid(defaultLayout.dependants))
    {
        m_currentLayoutId = defaultLayout.id;
    }

    return m_currentLayoutId;
}

std::vector<int> XiboLayoutScheduler::nextOverlayLayoutsIds()
{
    std::vector<int> overlayLayoutsIds;

    for(auto&& layout : m_schedule.overlayLayouts())
    {
        if(isLayoutOnSchedule(layout) && isLayoutValid(layout.dependants))
        {
            overlayLayoutsIds.emplace_back(layout.id);
        }
    }

    return overlayLayoutsIds;
}

int XiboLayoutScheduler::currentLayoutId() const
{
    return m_currentLayoutId;
}

SchedulerStatus XiboLayoutScheduler::status()
{
    SchedulerStatus status;

    collectLayoutListStatus(status, m_schedule.regularLayouts());
    collectLayoutListStatus(status, m_schedule.overlayLayouts());

    status.generatedTime = m_schedule.generatedTime();
    status.currentLayout = m_currentLayoutId;

    return status;
}

template<typename LayoutsList>
void XiboLayoutScheduler::collectLayoutListStatus(SchedulerStatus& status, const LayoutsList& layouts)
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

int XiboLayoutScheduler::nextScheduledLayoutId()
{
    auto&& regularLayouts = m_schedule.regularLayouts();
    for(size_t i = 0; i != regularLayouts.size(); ++i)
    {
        auto&& layout = regularLayouts.nextLayout();

        if(isLayoutOnSchedule(layout) && isLayoutValid(layout.dependants))
        {
            return layout.id;
        }
    }

    return EmptyLayoutId;
}

bool XiboLayoutScheduler::isLayoutOnSchedule(const ScheduledLayout& layout) const
{
    auto currentDT = boost::posix_time::second_clock::local_time();

    if(currentDT >= layout.startDT && currentDT < layout.endDT)
    {
        return true;
    }

    return false;
}

bool XiboLayoutScheduler::isLayoutValid(const std::vector<std::string>& dependants) const
{
    for(auto&& dependant : m_schedule.globalDependants())
    {        
        if(!FileSystem::exists(Resources::directory() / dependant))
        {
            return false;
        }
    }

    for(auto&& dependant : dependants)
    {
        if(!FileSystem::exists(Resources::directory() / dependant))
        {
            return false;
        }
    }

    return true;
}
