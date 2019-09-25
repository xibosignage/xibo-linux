#include "TransitionExecutor.hpp"

TransitionExecutor::TransitionExecutor(Transition::Heading heading, int duration,
                                       const std::shared_ptr<IWidget>& media) :
    m_heading(heading),
    m_duration(duration),
    m_media(media),
    m_timer(std::make_unique<Timer>())
{
}

SignalFinished& TransitionExecutor::finished()
{
    return m_finished;
}

Transition::Heading TransitionExecutor::heading() const
{
    return m_heading;
}

int TransitionExecutor::duration() const
{
    return m_duration;
}

std::shared_ptr<IWidget> TransitionExecutor::media() const
{
    return m_media;
}

Timer& TransitionExecutor::timer()
{
    return *m_timer;
}
