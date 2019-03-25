#pragma once

#include "IEventPublisher.hpp"
#include "CallbackEventQueue.hpp"

#include <map>

template<typename Interface = IEventPublisher>
class EventPublisher : public Interface
{
    static_assert(std::is_base_of_v<IEventPublisher, Interface>, "Should implement IObservable");

public:
    void subscribe(EventType type, const EventHandler& handler) final
    {
        m_observers.emplace(type, handler);
    }

    template<typename Event>
    void pushEvent(const Event& ev)
    {
        static_assert(std::is_copy_assignable_v<Event> && std::is_copy_constructible_v<Event>, "Event should copy constructible and copy assignable");

        auto iterRange = m_observers.equal_range(ev.type());

        for(auto it = iterRange.first; it != iterRange.second; ++it)
        {
            callbackQueue().add(std::bind(it->second, ev));
        }
    }

private:
    std::multimap<EventType, EventHandler> m_observers;

};
