#include "FlyTransitionExecutor.hpp"

FlyTransitionExecutor::FlyTransitionExecutor(Transition::Heading heading, Transition::Direction direction, int duration,
                                             const std::shared_ptr<IWidget>& media) :
    TransitionExecutor(heading, duration, media),
    m_direction(direction)
{
}

#include "common/logger/Logging.hpp"

void FlyTransitionExecutor::apply()
{
    std::string h = heading() == Transition::Heading::In ? "in" : "out";

    Log::debug("Executing fly {} transition with {} duration", h, duration());
}
