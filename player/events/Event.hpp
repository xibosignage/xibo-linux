#pragma once

#include <functional>

enum class EventType
{
    StartMedia,
    StopMedia,
    DurationExpired,
    PlaybackFinished,
    CollectionFinished,
    WidgetShown
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
    StartMediaEvent(int id);

    int id() const;
    EventType type() const override;

private:
    int m_id;

};

class StopMediaEvent : public Event
{
public:
    StopMediaEvent(int id);

    int id() const;
    EventType type() const override;

private:
    int m_id;

};

class DurationExpiredEvent : public Event
{
public:
    EventType type() const override;
};

class PlaybackFinishedEvent : public Event
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

#include "managers/CollectionResult.hpp"

class CollectionFinishedEvent : public Event
{
public:
    CollectionFinishedEvent(const CollectionResult& result);
    EventType type() const override;
    const CollectionResult& result() const;

private:
    CollectionResult m_result;

};

class WidgetShownEvent : public Event
{
public:
    EventType type() const override;

};
