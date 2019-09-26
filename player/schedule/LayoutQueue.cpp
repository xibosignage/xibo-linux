#include "LayoutQueue.hpp"

const int DefaultPriority = 0;

void LayoutQueue::clear()
{
    queue_.clear();
}

size_t LayoutQueue::size() const
{
    return queue_.size();
}

bool LayoutQueue::empty() const
{
    return queue_.empty();
}

LayoutQueue::ConstIterator LayoutQueue::begin() const
{
    return queue_.begin();
}

LayoutQueue::ConstIterator LayoutQueue::end() const
{
    return queue_.end();
}

const std::vector<ScheduledLayout>& LayoutQueue::queue() const
{
    return queue_;
}

const ScheduledLayout& LayoutQueue::at(size_t index) const
{
    return queue_.at(index);
}

int LayoutQueue::highestPriority() const
{
    if (empty()) return DefaultPriority;

    return at(FirstItemIndex).priority;
}
