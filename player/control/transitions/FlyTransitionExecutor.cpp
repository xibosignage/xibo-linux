#include "FlyTransitionExecutor.hpp"

#include "common/Utils.hpp"
#include "common/logger/Logging.hpp"

FlyTransitionExecutor::FlyTransitionExecutor(Transition::Heading heading,
                                             Transition::Direction direction,
                                             int duration,
                                             const std::shared_ptr<Xibo::Widget>& media) :
    TransitionExecutor(heading, duration, media),
    direction_(direction)
{
}

void FlyTransitionExecutor::apply()
{
    Log::debug("[FlyTransition] Executing {} transition with {} duration", Utils::toString(heading()), duration());
}
