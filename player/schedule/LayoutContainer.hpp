#pragma once

#include "ScheduleItem.hpp"

#include <vector>
#include <algorithm>

class LayoutContainer
{
public:
    using ConstIterator = std::vector<ScheduledLayout>::const_iterator;

    void add(ScheduledLayout&& layout);
    void clear();

    size_t size() const;

    ConstIterator begin() const;
    ConstIterator end() const;

protected:
    std::vector<ScheduledLayout> m_list;

};

template<typename Base>
class SequentialLayoutList : public Base
{
public:
    ScheduledLayout next() const
    {
        size_t currentIndex = m_nextIndex;

        m_nextIndex = increaseIndex(currentIndex);

        return this->m_list[currentIndex];
    }

private:
    const size_t FirstItemIndex = 0;

    size_t increaseIndex(std::size_t index) const
    {
        size_t nextIndex = index + 1;

        if(nextIndex >= this->m_list.size())
            return FirstItemIndex;

        return nextIndex;
    }

private:
    mutable size_t m_nextIndex = FirstItemIndex;

};

template<typename BaseList>
class PriorityLayoutList : public BaseList
{
public:
    void add(ScheduledLayout&& layout)
    {
        if(layout.priority >= highestPriority())
        {
            if(layout.priority > highestPriority())
            {
                BaseList::clear();
            }
            BaseList::add(std::move(layout));
        }
    }

private:
    const int DefaultPriority = 0;

    int highestPriority() const
    {
        if(this->m_list.empty())
            return DefaultPriority;

        return this->m_list.front().priority;
    }
};


using RegularLayoutList = SequentialLayoutList<PriorityLayoutList<LayoutContainer>>;
using OverlayLayoutList = PriorityLayoutList<LayoutContainer>;
