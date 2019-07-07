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

int XiboLayoutScheduler::currentLayoutId() const
{
    return m_currentLayoutId;
}

SchedulerStatus XiboLayoutScheduler::status()
{
    SchedulerStatus status;

    for(auto&& layout : m_schedule.layouts())
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

    status.generatedTime = m_schedule.generatedTime();
    status.currentLayout = m_currentLayoutId;

    return status;
}

int XiboLayoutScheduler::nextScheduledLayoutId()
{
    for(size_t i = 0; i != m_schedule.scheduledLayoutsCount(); ++i)
    {
        auto&& layout = m_schedule.nextScheduledLayout();

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
