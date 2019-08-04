#pragma once

#include "LayoutQueue.hpp"

template<typename BaseQueue>
class PriorityLayoutQueue : public BaseQueue
{
public:
    template<typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, ScheduledLayout>>>
    void add(T&& layout)
    {
        if(layout.priority >= highestPriority())
        {
            if(layout.priority > highestPriority())
            {
                BaseQueue::clear();
            }
            BaseQueue::add(std::forward<T>(layout));
        }
    }

    std::vector<int> next() const
    {
        std::vector<int> ids;

        for(auto&& layout : this->queue())
        {
            ids.push_back(layout.id);
        }

        return ids;
    }

private:
    int DefaultPriority = 0;

    int highestPriority() const
    {
        if(this->empty())
            return DefaultPriority;

        return this->at(FirstItemIndex).priority;
    }
};
