#pragma once

#include "IObserver.hpp"
#include "IObservable.hpp"
#include "CallbackGlobalQueue.hpp"

#include <map>

template<typename Interface = IObservable>
class Observable : public Interface
{
    static_assert(std::is_base_of_v<IObservable, Interface>, "Should implement IObservable");

public:
    void subscribe(EventType type, const EventHandler& handler) final
    {
        m_observers.emplace(type, handler);
    }

    void pushEvent(Event* ev) final
    {
        auto iterRange = m_observers.equal_range(ev->type());

        for(auto it = iterRange.first; it != iterRange.second; ++it)
        {
            callbackQueue().add(std::bind(it->second, std::cref(*ev)));
        }
    }

private:
    std::multimap<EventType, EventHandler> m_observers;

};
