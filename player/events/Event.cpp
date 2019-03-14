#include "Event.hpp"

StartMediaEvent::StartMediaEvent(int id) :
    m_id(id)
{
}

int StartMediaEvent::id() const
{
    return m_id;
}

EventType StartMediaEvent::type() const
{
    return EventType::StartMedia;
}

StopMediaEvent::StopMediaEvent(int id) :
    m_id(id)
{
}

int StopMediaEvent::id() const
{
    return m_id;
}

EventType StopMediaEvent::type() const
{
    return EventType::StopMedia;
}

EventType DurationExpiredEvent::type() const
{
    return EventType::DurationExpired;
}

EventType PlaybackFinishedEvent::type() const
{
    return EventType::PlaybackFinished;
}

RegionDurationExpiredEvent::RegionDurationExpiredEvent(int id) :
    m_id(id)
{
}

int RegionDurationExpiredEvent::id() const
{
    return m_id;
}

CollectionFinishedEvent::CollectionFinishedEvent(const CollectionResult& result)
{
    m_result = result;
}

EventType CollectionFinishedEvent::type() const
{
    return EventType::CollectionFinished;
}

const CollectionResult& CollectionFinishedEvent::result() const
{
    return m_result;
}

EventType WidgetShownEvent::type() const
{
    return EventType::WidgetShown;
}
