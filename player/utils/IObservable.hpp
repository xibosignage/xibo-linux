#pragma once

#include "Event.hpp"

class IObservable
{
public:
    virtual ~IObservable() = default;
    virtual void subscribe(EventType type, const EventHandler& handler) = 0;
    virtual void pushEvent(const Event& ev) = 0;
};
