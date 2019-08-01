#include "LayoutContainer.hpp"

void LayoutContainer::addLayout(ScheduledLayout&& layout)
{
    m_layoutList.emplace_back(std::move(layout));
}

void LayoutContainer::clear()
{
    m_layoutList.clear();
}

size_t LayoutContainer::size() const
{
    return m_layoutList.size();
}

LayoutContainer::ConstIterator LayoutContainer::begin() const
{
    return m_layoutList.begin();
}

LayoutContainer::ConstIterator LayoutContainer::end() const
{
    return m_layoutList.end();
}
