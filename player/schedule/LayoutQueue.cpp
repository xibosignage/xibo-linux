#include "LayoutQueue.hpp"

const int DefaultPriority = 0;

void LayoutQueue::clear()
{
    m_queue.clear();
}

size_t LayoutQueue::size() const
{
    return m_queue.size();
}

bool LayoutQueue::empty() const
{
    return m_queue.empty();
}

LayoutQueue::ConstIterator LayoutQueue::begin() const
{
    return m_queue.begin();
}

LayoutQueue::ConstIterator LayoutQueue::end() const
{
    return m_queue.end();
}

const std::vector<ScheduledLayout>& LayoutQueue::queue() const
{
    return m_queue;
}

const ScheduledLayout& LayoutQueue::at(size_t index) const
{
    return m_queue.at(index);
}

int LayoutQueue::highestPriority() const
{
    if (empty()) return DefaultPriority;

    return at(FirstItemIndex).priority;
}
