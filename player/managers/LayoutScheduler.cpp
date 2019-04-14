#include "LayoutScheduler.hpp"

#include "common/logger/Logging.hpp"
#include "common/FileSystem.hpp"
#include "utils/Resources.hpp"

const size_t FIRST_ITEM_INDEX = 0;

LayoutScheduler::LayoutScheduler()
{
}

void LayoutScheduler::update(const LayoutSchedule& schedule)
{
    resetSchedule();

    m_defaultLayout = schedule.defaultLayout;
    m_globalDependants = std::move(schedule.globalDependants);
    fillScheduleItems(schedule.layouts);
}

void LayoutScheduler::resetSchedule()
{
//    m_nextLayoutIndex = FIRST_ITEM_INDEX;
    m_layouts.clear();
}

void LayoutScheduler::fillScheduleItems(const std::vector<ScheduledLayout>& scheduledItems)
{
    if(scheduledItems.empty()) return;

    int maxShowPriority = findMaxShowPriority(scheduledItems);
    Log::debug("Max show priority: {}", maxShowPriority);

    for(auto layout : scheduledItems)
    {
        Log::debug("Layout ID: {} SatartDT: {} EndDT: {} Priority: {}", layout.id, layout.startDT, layout.endDT, layout.priority);
        if(layout.priority == maxShowPriority)
        {
            m_layouts.push_back(layout);
        }
    }
}

int LayoutScheduler::findMaxShowPriority(const std::vector<ScheduledLayout>& scheduledItems)
{
    assert(scheduledItems.size() > 0);

    auto it = std::max_element(scheduledItems.begin(), scheduledItems.end(), [=](const auto& first, const auto& second){
        return first.priority < second.priority;
    });

    return it->priority;
}

int LayoutScheduler::nextLayoutId()
{
    if(!m_layouts.empty())
    {
        return nextValidLayoutId();
    }
    else
    {
        return m_defaultLayout.id;
    }
}

int LayoutScheduler::nextValidLayoutId()
{
    std::size_t indexCount = 0;
    std::size_t layoutIndex = m_nextLayoutIndex;

    while(indexCount != m_layouts.size())
    {
        auto&& nextLayout = m_layouts[layoutIndex];

        if(isLayoutOnSchedule(nextLayout) && allFilesExist(nextLayout.dependants))
        {
            m_nextLayoutIndex = increaseLayoutIndex(layoutIndex);
            return nextLayout.id;
        }

        layoutIndex = increaseLayoutIndex(layoutIndex);
        ++indexCount;
    }

    return m_defaultLayout.id;
}


bool LayoutScheduler::isLayoutOnSchedule(const ScheduledLayout& layout) const
{
    auto currentDT = boost::posix_time::second_clock::local_time();
    if(currentDT >= layout.startDT && currentDT < layout.endDT)
    {
        return true;
    }
    return false;
}

bool LayoutScheduler::allFilesExist(const std::vector<std::string>& dependants) const
{
    for(auto dependant : m_globalDependants)
    {
        if(!FileSystem::exists(Resources::directory() / dependant))
        {
            return false;
        }
    }

    for(auto dependant : dependants)
    {
        if(!FileSystem::exists(Resources::directory() / dependant))
        {
            return false;
        }
    }

    return true;
}

size_t LayoutScheduler::increaseLayoutIndex(std::size_t index) const
{
    size_t nextLayoutIndex = index + 1;

    if(nextLayoutIndex >= m_layouts.size())
        return FIRST_ITEM_INDEX;

    return nextLayoutIndex;
}
