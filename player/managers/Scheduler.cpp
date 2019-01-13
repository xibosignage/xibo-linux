#include "Scheduler.hpp"

#include "control/IMainLayout.hpp"
#include "creators/MainDirector.hpp"
#include "utils/Utilities.hpp"
#include "utils/Logger.hpp"
#include "utils/Resources.hpp"
#include "constants.hpp"

#include <boost/property_tree/ptree.hpp>
#include <filesystem>

const size_t FIRST_ITEM_INDEX = 0;

void Scheduler::update(const Schedule::Result& schedule)
{
    resetSchedule();

    m_defaultLayout = schedule.defaultLayout;
    m_globalDependants = std::move(schedule.globalDependants);
    fillScheduleItems(schedule.layouts);
}

void Scheduler::resetSchedule()
{
//    m_nextLayoutIndex = FIRST_ITEM_INDEX; // FIXME (should I reset index ???)
    m_layouts.clear();
}

void Scheduler::fillScheduleItems(const std::vector<ScheduledLayout>& scheduledItems)
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

int Scheduler::findMaxShowPriority(const std::vector<ScheduledLayout>& scheduledItems)
{
    assert(scheduledItems.size() > 0);

    auto it = std::max_element(scheduledItems.begin(), scheduledItems.end(), [=](const auto& first, const auto& second){
        return first.priority < second.priority;
    });

    return it->priority;
}

std::unique_ptr<IMainLayout> Scheduler::nextLayout()
{
    MainDirector director;
    auto layout = director.buildLayoutWithChildren(nextLayoutToPlayId());

    layout->subscribe(EventType::DurationExpired, [this](const Event&){
        Log::debug("Got event layout expired");

        pushEvent(LayoutExpiredEvent{});
    });

    return layout;
}

int Scheduler::nextLayoutToPlayId()
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

int Scheduler::nextValidLayoutId()
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


bool Scheduler::isLayoutOnSchedule(const ScheduledLayout& layout) const
{
    auto currentDT = boost::posix_time::second_clock::local_time();
    if(currentDT >= layout.startDT && currentDT < layout.endDT)
    {
        return true;
    }
    return false;
}

bool Scheduler::allFilesExist(const std::vector<std::string>& dependants) const
{
    for(auto dependant : m_globalDependants)
    {
        if(!std::filesystem::exists(Resources::directory() / dependant))
        {
            return false;
        }
    }

    for(auto dependant : dependants)
    {
        if(!std::filesystem::exists(Resources::directory() / dependant))
        {
            return false;
        }
    }

    return true;
}

size_t Scheduler::increaseLayoutIndex(std::size_t index) const
{
    size_t nextLayoutIndex = index + 1;

    if(nextLayoutIndex >= m_layouts.size())
        return FIRST_ITEM_INDEX;

    return nextLayoutIndex;
}
