#pragma once

#include "LayoutQueue.hpp"

class RegularLayoutQueue : public LayoutQueue
{
public:
    template <typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, DefaultScheduledLayout>>>
    void addDefault(T&& layout)
    {
        m_defaultLayout = std::forward<T>(layout);
    }

    void updateCurrent(LayoutId id);
    LayoutId next() const;
    LayoutId current() const;
    bool inQueue(LayoutId id) const;

private:
    const ScheduledLayout& nextRegularLayout() const;
    size_t increaseIndex(std::size_t index) const;
    boost::optional<size_t> layoutIndexBy(LayoutId id) const;

private:
    boost::optional<DefaultScheduledLayout> m_defaultLayout;
    mutable size_t m_nextIndex = FirstItemIndex;
    mutable LayoutId m_currentId = EmptyLayoutId;
};
