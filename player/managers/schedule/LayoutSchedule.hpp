#pragma once

#include "ScheduleItem.hpp"
#include "ScheduledLayoutList.hpp"

class LayoutSchedule
{
public:
    void updateDependants(std::vector<std::string>&& globalDependants);
    const std::vector<std::string>& globalDependants() const;

    void updateDefaultLayout(DefaultScheduledLayout&& defaultLayout);
    const DefaultScheduledLayout& defaultLayout() const;

    void addScheduledLayout(ScheduledLayout&& layout);
    const RegularLayoutsList& regularLayouts() const;

    void addOverlayLayouts(OverlayLayoutsList&& layouts);
    const OverlayLayoutsList& overlayLayouts() const;

    void updateGeneratedTime(const std::string& dt);
    std::string generatedTime() const;

private:
    std::string m_generatedTime;
    std::vector<std::string> m_globalDependants;
    RegularLayoutsList m_regularLayouts;
    OverlayLayoutsList m_overlayLayouts;
    DefaultScheduledLayout m_defaultLayout;

};
