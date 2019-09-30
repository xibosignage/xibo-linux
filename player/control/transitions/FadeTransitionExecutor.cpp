#include "FadeTransitionExecutor.hpp"

const int DefaultTimerDuration = 100;
const double MaxOpacity = 1.0;
const double MinOpacity = 0.0;

FadeTransitionExecutor::FadeTransitionExecutor(Transition::Heading heading,
                                               int duration,
                                               const std::shared_ptr<Xibo::Widget>& media) :
    TransitionExecutor(heading, duration, media)
{
}

void FadeTransitionExecutor::apply()
{
    int count = duration() / DefaultTimerDuration;
    double step = MaxOpacity / count;
    init();

    timer().start(std::chrono::milliseconds(DefaultTimerDuration), [this, step]() {
        double value = nextValue(media()->opacity(), step);
        media()->setOpacity(value);

        if (isFinished(value))
        {
            finished()();
            return false;
        }

        return true;
    });
}

void FadeTransitionExecutor::init()
{
    if (heading() == Transition::Heading::In)
    {
        media()->setOpacity(MinOpacity);
    }
    else
    {
        media()->setOpacity(MaxOpacity);
    }
}

bool FadeTransitionExecutor::isFinished(double value) const
{
    return heading() == Transition::Heading::In ? value >= MaxOpacity : value <= MinOpacity;
}

double FadeTransitionExecutor::nextValue(double oldValue, double step) const
{
    return heading() == Transition::Heading::In ? oldValue + step : oldValue - step;
}
