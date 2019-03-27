#pragma once

#include <functional>

enum class EventType
{
    DurationExpired,
    CollectionFinished,
    SettingsUpdated,
    ScheduleUpdated,
    WidgetShown,
    KeyPress,
    ButtonClicked
};

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType type() const = 0;
};

using EventHandler = std::function<void(const Event&)>;

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

#include "model/PlayerSettings.hpp"

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

class WidgetShownEvent : public Event
{
public:
    EventType type() const override;

};

class KeyPressEvent : public Event
{
public:
    KeyPressEvent(unsigned int value, const std::string& string);
    EventType type() const override;
    unsigned int value() const;
    std::string string() const;

private:
    unsigned int m_value;
    std::string m_string;

};

class ButtonClickedEvent : public Event
{
public:
    EventType type() const override;

};
