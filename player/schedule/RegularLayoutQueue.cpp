#include "RegularLayoutQueue.hpp"

bool RegularLayoutQueue::inQueue(int id) const
{
    if(!empty()) return layoutIndexBy(id).has_value();
    if(m_defaultLayout) return m_defaultLayout->id == id;

    return false;
}

void RegularLayoutQueue::setCurrentLayout(int id)
{
    auto index = layoutIndexBy(id);
    if(index)
    {
        m_nextIndex = increaseIndex(index.value());
    }
}

int RegularLayoutQueue::next() const
{
    if(!empty())
    {
        return nextRegularLayout().id;
    }
    return m_defaultLayout ? m_defaultLayout->id : EmptyLayoutId;
}

const ScheduledLayout& RegularLayoutQueue::nextRegularLayout() const
{
    size_t currentIndex = m_nextIndex;

    m_nextIndex = increaseIndex(currentIndex);

    return at(currentIndex);
}

size_t RegularLayoutQueue::increaseIndex(std::size_t index) const
{
    size_t nextIndex = index + 1;

    if(nextIndex >= size())
        return FirstItemIndex;

    return nextIndex;
}

boost::optional<size_t> RegularLayoutQueue::layoutIndexBy(int id) const
{
    for(size_t index = 0; index != size(); ++index)
    {
        if(at(index).id == id)
            return index;
    }
    return {};
}
