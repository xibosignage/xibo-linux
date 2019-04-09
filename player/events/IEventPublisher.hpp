#pragma once

#include "Event.hpp"

class IEventPublisher
{
public:
    virtual ~IEventPublisher() = default;
    virtual void subscribe(EventType type, const EventHandler& handler) = 0;
};
