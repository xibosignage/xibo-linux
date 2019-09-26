#pragma once

#include "TransitionExecutor.hpp"

class FlyTransitionExecutor : public TransitionExecutor
{
public:
    FlyTransitionExecutor(Transition::Heading heading, Transition::Direction direction, int duration,
                          const std::shared_ptr<IWidget>& media);

    void apply() override;

private:
    Transition::Direction m_direction;
};
