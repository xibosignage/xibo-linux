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

ScaleEvent::ScaleEvent(double scaleX, double scaleY) : m_scaleX(scaleX), m_scaleY(scaleY)
{
}

EventType ScaleEvent::type() const
{
    return EventType::ScaleMedia;
}

double ScaleEvent::scaleX() const
{
    return m_scaleX;
}

double ScaleEvent::scaleY() const
{
    return m_scaleY;
}
