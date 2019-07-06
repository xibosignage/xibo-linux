#include "LayoutSchedule.hpp"

void LayoutSchedule::updateDependants(std::vector<std::string>&& globalDependants)
{
    m_globalDependants = std::move(globalDependants);
}

const std::vector<std::string>& LayoutSchedule::globalDependants() const
{
    return m_globalDependants;
}

void LayoutSchedule::updateDefaultLayout(DefaultScheduledLayout&& defaultLayout)
{
    m_defaultLayout = std::move(defaultLayout);
}

const DefaultScheduledLayout& LayoutSchedule::defaultLayout() const
{
    return m_defaultLayout;
}

void LayoutSchedule::addScheduledLayout(ScheduledLayout&& layout)
{
    m_regularLayouts.addLayout(std::move(layout));
}

const RegularLayoutsList& LayoutSchedule::regularLayouts() const
{
    return m_regularLayouts;
}

void LayoutSchedule::addOverlayLayouts(OverlayLayoutsList&& layouts)
{
    m_overlayLayouts = std::move(layouts);
}

const OverlayLayoutsList& LayoutSchedule::overlayLayouts() const
{
    return m_overlayLayouts;
}

void LayoutSchedule::updateGeneratedTime(const std::string& dt)
{
    m_generatedTime = dt;
}

std::string LayoutSchedule::generatedTime() const
{
    return m_generatedTime;
}
