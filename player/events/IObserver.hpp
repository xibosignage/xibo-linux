#pragma once

class Event;

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void handleEvent(const Event& ev) = 0;
};
