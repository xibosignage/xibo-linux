#pragma once

#include "control/transitions/TransitionExecutor.hpp"

class FlyTransitionExecutor : public TransitionExecutor
{
public:
    FlyTransitionExecutor(Transition::Heading heading,
                          Transition::Direction direction,
                          int duration,
                          const std::shared_ptr<Xibo::Widget>& media);

    void apply() override;

private:
    Transition::Direction direction_;
};
