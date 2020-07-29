#include "RegularLayoutQueue.hpp"

bool RegularLayoutQueue::inQueue(LayoutId id) const
{
    if (!empty()) return layoutIndexBy(id).has_value();
    if (defaultLayout_) return defaultLayout_->id == id;

    return false;
}

void RegularLayoutQueue::updateCurrent(LayoutId id)
{
    auto index = layoutIndexBy(id);
    if (index)
    {
        nextIndex_ = increaseIndex(index.value());
    }
    if (index || defaultLayout_->id == id)
    {
        currentId_ = id;
    }
}

LayoutId RegularLayoutQueue::next() const
{
    if (!empty())
    {
        currentId_ = nextRegularLayout().id;
    }
    else
    {
        currentId_ = defaultLayout_ ? defaultLayout_->id : EmptyLayoutId;
    }

    return currentId_;
}

LayoutId RegularLayoutQueue::current() const
{
    return currentId_;
}

const ScheduledLayout& RegularLayoutQueue::nextRegularLayout() const
{
    size_t currentIndex = nextIndex_;

    nextIndex_ = increaseIndex(currentIndex);

    return at(currentIndex);
}

size_t RegularLayoutQueue::increaseIndex(std::size_t index) const
{
    size_t nextIndex = index + 1;

    if (nextIndex >= size()) return FirstItemIndex;

    return nextIndex;
}

boost::optional<size_t> RegularLayoutQueue::layoutIndexBy(LayoutId id) const
{
    for (size_t index = 0; index != size(); ++index)
    {
        if (at(index).id == id) return index;
    }
    return {};
}
