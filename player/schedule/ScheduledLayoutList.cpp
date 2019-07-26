#include "ScheduledLayoutList.hpp"

void LayoutList::addLayout(ScheduledLayout&& layout)
{
    m_layoutList.emplace_back(std::move(layout));
}

void LayoutList::clear()
{
    m_layoutList.clear();
}

size_t LayoutList::size() const
{
    return m_layoutList.size();
}

LayoutList::ConstIterator LayoutList::begin() const
{
    return m_layoutList.begin();
}

LayoutList::ConstIterator LayoutList::end() const
{
    return m_layoutList.end();
}
