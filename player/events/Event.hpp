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
    LayoutExpired,
    CollectionFinished,
    SettingsUpdated,
    ScheduleUpdated
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

class RegionDurationExpiredEvent : public DurationExpiredEvent
{
public:
    RegionDurationExpiredEvent(int id);
    int id() const;

private:
    int m_id;
};

#include "utils/PlayerError.hpp"

class CollectionFinishedEvent : public Event
{
public:
    CollectionFinishedEvent(const PlayerError& error);
    EventType type() const override;
    const PlayerError& error() const;

private:
    PlayerError m_error;

};

#include "control/PlayerSettings.hpp"

class SettingsUpdatedEvent : public Event
{
public:
    SettingsUpdatedEvent(const PlayerSettings& settings);
    EventType type() const override;
    const PlayerSettings& settings() const;

private:
    PlayerSettings m_settings;

};

#include "managers/LayoutSchedule.hpp"

class ScheduleUpdatedEvent : public Event
{
public:
    ScheduleUpdatedEvent(const LayoutSchedule& schedule);
    EventType type() const override;
    const LayoutSchedule& schedule() const;

private:
    LayoutSchedule m_schedule;

};

