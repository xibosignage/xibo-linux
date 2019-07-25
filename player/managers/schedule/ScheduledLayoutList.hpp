#pragma once

#include "ScheduleItem.hpp"

#include <vector>
#include <algorithm>

class LayoutsList
{
public:
    using ConstIterator = std::vector<ScheduledLayout>::const_iterator;

    void addLayout(ScheduledLayout&& layout);
    size_t size() const;

    ConstIterator begin() const;
    ConstIterator end() const;

protected:
    std::vector<ScheduledLayout> m_layoutList;

};

template<typename Base>
class SortedLayoutsList : public Base
{
public:
    void addLayout(ScheduledLayout&& layout)
    {
        Base::addLayout(std::move(layout));

        std::stable_sort(this->m_layoutList.begin(), this->m_layoutList.end(), [](const auto& first, const auto& second){
            return first.priority > second.priority;
        });
    }

};

const size_t FirstItemIndex = 0;

template<typename Base>
class SequentialLayoutsList : public Base
{
public:
    ScheduledLayout nextLayout() const
    {
        size_t layoutIndex = m_nextLayoutIndex;

        m_nextLayoutIndex = increaseLayoutIndex(layoutIndex);

        return this->m_layoutList[layoutIndex];
    }

private:
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

using RegularLayoutsList = SequentialLayoutsList<SortedLayoutsList<LayoutsList>>;
using OverlayLayoutsList = SortedLayoutsList<LayoutsList>;
