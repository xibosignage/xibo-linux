#pragma once

class Event;

class ISubscriber
{
public:
    virtual ~ISubscriber() = default;
    virtual void handleEvent(const Event& ev) = 0;
};
