#pragma once

#include "schedule/ScheduleItem.hpp"

#include <algorithm>
#include <boost/optional/optional.hpp>
#include <vector>

const size_t FirstItemIndex = 0;

class LayoutQueue
{
public:
    using ConstIterator = std::vector<ScheduledLayout>::const_iterator;

    template <typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, ScheduledLayout>>>
    void add(T&& layout)
    {
        if (layout.priority >= highestPriority())
        {
            if (layout.priority > highestPriority())
            {
                clear();
            }
            queue_.emplace_back(std::forward<T>(layout));
        }
    }

    void clear();
    bool empty() const;
    size_t size() const;

    ConstIterator begin() const;  // TODO tests
    ConstIterator end() const;    // TODO tests

protected:
    ~LayoutQueue() = default;

    const std::vector<ScheduledLayout>& queue() const;
    const ScheduledLayout& at(size_t index) const;

private:
    int highestPriority() const;

private:
    std::vector<ScheduledLayout> queue_;
};
