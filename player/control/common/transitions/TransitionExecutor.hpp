#pragma once

#include "../IWidget.hpp"
#include "Transition.hpp"
#include "common/dt/Timer.hpp"

#include <memory>

using SignalFinished = boost::signals2::signal<void()>;

class TransitionExecutor
{
public:
    TransitionExecutor(Transition::Heading heading, int duration, const std::shared_ptr<IWidget>& media);
    virtual ~TransitionExecutor() = default;

    virtual void apply() = 0;

    SignalFinished& finished();

protected:
    Transition::Heading heading() const;
    int duration() const;
    std::shared_ptr<IWidget> media() const;
    Timer& timer();

private:
    Transition::Heading m_heading;
    int m_duration;
    std::shared_ptr<IWidget> m_media;
    std::unique_ptr<Timer> m_timer;
    SignalFinished m_finished;
};
