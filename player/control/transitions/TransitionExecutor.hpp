#pragma once

#include "common/dt/Timer.hpp"
#include "control/transitions/Transition.hpp"
#include "control/widgets/Widget.hpp"

#include <memory>

using SignalFinished = boost::signals2::signal<void()>;

class TransitionExecutor
{
public:
    TransitionExecutor(Transition::Heading heading, int duration, const std::shared_ptr<Xibo::Widget>& media);
    virtual ~TransitionExecutor() = default;

    virtual void apply() = 0;

    SignalFinished& finished();

protected:
    Transition::Heading heading() const;
    int duration() const;
    std::shared_ptr<Xibo::Widget> media() const;
    Timer& timer();

private:
    Transition::Heading heading_;
    int duration_;
    std::shared_ptr<Xibo::Widget> media_;
    std::unique_ptr<Timer> timer_;
    SignalFinished finished_;
};
