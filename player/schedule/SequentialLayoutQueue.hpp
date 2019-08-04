#pragma once

#include "LayoutQueue.hpp"

template<typename BaseQueue>
class SequentialLayoutQueue : public BaseQueue
{
public:
    template<typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, DefaultScheduledLayout>>>
    void addDefault(T&& layout)
    {
        m_defaultLayout = std::forward<T>(layout);
    }

    int next() const
    {
        if(this->size() > 0)
        {
            return nextRegularLayout().id;
        }

        return m_defaultLayout ? m_defaultLayout->id : EmptyLayoutId;
    }

private:
    const ScheduledLayout& nextRegularLayout() const
    {
        size_t currentIndex = m_nextIndex;

        m_nextIndex = increaseIndex(currentIndex);

        return this->at(currentIndex);
    }

    size_t increaseIndex(std::size_t index) const
    {
        size_t nextIndex = index + 1;

        if(nextIndex >= this->size())
            return FirstItemIndex;

        return nextIndex;
    }

private:
    boost::optional<DefaultScheduledLayout> m_defaultLayout;
    mutable size_t m_nextIndex = FirstItemIndex;

};
