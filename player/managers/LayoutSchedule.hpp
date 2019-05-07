#pragma once

#include "ScheduleItem.hpp"

class LayoutSchedule
{
public:
    LayoutSchedule();

    void updateDependants(std::vector<std::string>&& globalDependants);
    const std::vector<std::string>& globalDependants() const;

    void updateDefaultLayout(DefaultScheduledLayout&& defaultLayout);
    const DefaultScheduledLayout& defaultLayout() const;

    void updateScheduledLayouts(std::vector<ScheduledLayout>&& layouts);
    const std::vector<ScheduledLayout>& layouts() const;
    const ScheduledLayout& nextScheduledLayout();

    void updateGeneratedTime(const std::string& dt);
    std::string generatedTime() const;

    size_t scheduledLayoutsCount() const;

private:
    size_t increaseLayoutIndex(std::size_t index) const;

private:
    size_t m_nextLayoutIndex;
    std::string m_generatedTime;
    std::vector<std::string> m_globalDependants;
    std::vector<ScheduledLayout> m_scheduledLayouts;
    DefaultScheduledLayout m_defaultLayout;

};
