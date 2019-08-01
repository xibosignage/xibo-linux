#pragma once

#include "ScheduleItem.hpp"

#include <vector>
#include <algorithm>

class LayoutContainer
{
public:
    using ConstIterator = std::vector<ScheduledLayout>::const_iterator;

    void addLayout(ScheduledLayout&& layout);
    void clear();

    size_t size() const;

    ConstIterator begin() const;
    ConstIterator end() const;

protected:
    std::vector<ScheduledLayout> m_layoutList;

};

template<typename Base>
class SequentialLayoutList : public Base
{
public:
    ScheduledLayout nextLayout() const
    {
        size_t layoutIndex = m_nextLayoutIndex;

        m_nextLayoutIndex = increaseLayoutIndex(layoutIndex);

        return this->m_layoutList[layoutIndex];
    }

private:
    const size_t FirstItemIndex = 0;

    size_t increaseLayoutIndex(std::size_t index) const
    {
        size_t nextLayoutIndex = index + 1;

        if(nextLayoutIndex >= this->m_layoutList.size())
            return FirstItemIndex;

        return nextLayoutIndex;
    }

private:
    mutable size_t m_nextLayoutIndex = FirstItemIndex;

};

template<typename Base>
class PriorityLayoutList : public Base
{
public:
    void addLayout(ScheduledLayout&& layout)
    {
        if(layout.priority >= highestPriority())
        {
            if(layout.priority > highestPriority())
            {
                Base::clear();
            }
            Base::addLayout(std::move(layout));
        }
    }

private:
    const int DefaultPriority = 0;

    int highestPriority() const
    {
        if(this->m_layoutList.empty())
            return DefaultPriority;

        return this->m_layoutList.front().priority;
    }
};


using RegularLayoutList = SequentialLayoutList<PriorityLayoutList<LayoutContainer>>;
using OverlayLayoutList = PriorityLayoutList<LayoutContainer>;
