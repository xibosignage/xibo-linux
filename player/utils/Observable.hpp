#pragma once

#include "IObserver.hpp"
#include "IObservable.hpp"

#include <map>

template<typename Interface>
class Observable : public Interface
{
    static_assert(std::is_base_of_v<IObservable, Interface>, "Should implement IObservable");

public:
    void subcribe(EventType type, const EventHandler& handler) override
    {
        m_observers.emplace(type, handler);
    }

    void pushEvent(const Event& ev) override
    {
        auto iterRange = m_observers.equal_range(ev.type());

        for(auto it = iterRange.first; it != iterRange.second; ++it)
        {
            it->second(ev);
        }
    }

private:
    std::multimap<EventType, EventHandler> m_observers;

};
