#include "LayoutContainer.hpp"

void LayoutContainer::add(ScheduledLayout&& layout)
{
    m_list.emplace_back(std::move(layout));
}

void LayoutContainer::clear()
{
    m_list.clear();
}

size_t LayoutContainer::size() const
{
    return m_list.size();
}

LayoutContainer::ConstIterator LayoutContainer::begin() const
{
    return m_list.begin();
}

LayoutContainer::ConstIterator LayoutContainer::end() const
{
    return m_list.end();
}
