#include "ScheduledLayoutList.hpp"

void LayoutsList::addLayout(ScheduledLayout&& layout)
{
    m_layoutList.emplace_back(std::move(layout));
}

size_t LayoutsList::size() const
{
    return m_layoutList.size();
}

std::vector<ScheduledLayout>::const_iterator LayoutsList::begin() const
{
    return m_layoutList.begin();
}

std::vector<ScheduledLayout>::const_iterator LayoutsList::end() const
{
    return m_layoutList.end();
}
