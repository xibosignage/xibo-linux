#include "Event.hpp"

EventType DurationExpiredEvent::type() const
{
    return EventType::DurationExpired;
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
