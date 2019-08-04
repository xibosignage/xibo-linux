#pragma once

#include "ScheduleItem.hpp"

#include <vector>
#include <algorithm>
#include <boost/optional/optional.hpp>

const size_t FirstItemIndex = 0;

class LayoutQueue
{
public:
    using ConstIterator = std::vector<ScheduledLayout>::const_iterator;

    template<typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, ScheduledLayout>>>
    void add(T&& layout)
    {
        m_queue.emplace_back(std::forward<T>(layout));
    }

    void clear();
    size_t size() const;
    bool empty() const;

    ConstIterator begin() const;
    ConstIterator end() const;

protected:
    const std::vector<ScheduledLayout>& queue() const;
    const ScheduledLayout& at(size_t index) const;

private:
    std::vector<ScheduledLayout> m_queue;

};
