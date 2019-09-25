#pragma once

#include "TransitionExecutor.hpp"

class FadeTransitionExecutor : public TransitionExecutor
{
public:
    FadeTransitionExecutor(Transition::Heading heading, int duration, const std::shared_ptr<IWidget>& media);

    void apply() override;

private:
    void init();
    bool isFinished(double value) const;
    double nextValue(double oldValue, double step) const;
};
