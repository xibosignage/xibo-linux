#pragma once

#include <sigc++/signal.h>

enum class EventType
{
    StartMedia,
    StopMedia,
    DurationExpired,
    ScaleMedia
};

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType type() const = 0;
};

using EventPublisher = sigc::signal<void(const Event&)>;

class StartMediaEvent : public Event
{
public:
    EventType type() const override;
};

class StopMediaEvent : public Event
{
public:
    EventType type() const override;
};

class DurationExpiredEvent : public Event
{
public:
    EventType type() const override;
};


class ScaleEvent : public Event
{
public:
    ScaleEvent(double scaleX, double scaleY);
    EventType type() const override;
    double scaleX() const;
    double scaleY() const;

private:
    double m_scaleX;
    double m_scaleY;
};
