#include "Media.hpp"

Media::Media(int id) : m_id(id)
{
}

int Media::id() const
{
    return m_id;
}

int Media::duration() const
{
    return m_duration;
}

void Media::setDuration(int duration)
{
    m_duration = duration;
}
