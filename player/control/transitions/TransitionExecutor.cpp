#include "TransitionExecutor.hpp"

TransitionExecutor::TransitionExecutor(Transition::Heading heading,
                                       int duration,
                                       const std::shared_ptr<Xibo::Widget>& media) :
    heading_(heading),
    duration_(duration),
    media_(media),
    timer_(std::make_unique<Timer>())
{
}

SignalFinished& TransitionExecutor::finished()
{
    return finished_;
}

Transition::Heading TransitionExecutor::heading() const
{
    return heading_;
}

int TransitionExecutor::duration() const
{
    return duration_;
}

std::shared_ptr<Xibo::Widget> TransitionExecutor::media() const
{
    return media_;
}

Timer& TransitionExecutor::timer()
{
    return *timer_;
}
