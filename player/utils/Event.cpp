#include "Event.hpp"

EventType StartMediaEvent::type() const
{
    return EventType::StartMedia;
}

EventType StopMediaEvent::type() const
{
    return EventType::StopMedia;
}

EventType DurationExpiredEvent::type() const
{
    return EventType::DurationExpired;
}

ScaleMediaEvent::ScaleMediaEvent(double scaleX, double scaleY) : m_scaleX(scaleX), m_scaleY(scaleY)
{
}

EventType ScaleMediaEvent::type() const
{
    return EventType::ScaleMedia;
}

double ScaleMediaEvent::scaleX() const
{
    return m_scaleX;
}

double ScaleMediaEvent::scaleY() const
{
    return m_scaleY;
}

EventType PlaybackFinishedEvent::type() const
{
    return EventType::PlaybackFinished;
}
