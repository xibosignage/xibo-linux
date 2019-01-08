#pragma once

#include <sigc++/signal.h>
#include <functional>

enum class EventType
{
    StartMedia,
    StopMedia,
    DurationExpired,
    ScaleMedia,
    PlaybackFinished,
    LayoutExpired
};

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType type() const = 0;
};

using EventHandler = std::function<void(const Event&)>;

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

class RegionDurationExpiredEvent : public DurationExpiredEvent
{
public:
    RegionDurationExpiredEvent(int id);
    int id() const;

private:
    int m_id;
};

class PlaybackFinishedEvent : public Event
{
public:
    EventType type() const override;
};

class LayoutExpiredEvent : public Event
{
public:
    EventType type() const override;
};
class ScaleMediaEvent : public Event
{
public:
    ScaleMediaEvent(double scaleX, double scaleY);
    EventType type() const override;
    double scaleX() const;
    double scaleY() const;

private:
    double m_scaleX;
    double m_scaleY;
};
