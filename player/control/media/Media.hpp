#pragma once

#include "IMedia.hpp"
#include "MediaOptions.hpp"

#include "common/dt/Timer.hpp"

#include "control/common/IWidget.hpp"
#include "control/common/transitions/TransitionExecutor.hpp"

class Media : public IMedia
{
public:
    Media(const MediaOptions& options, const std::shared_ptr<IWidget>& view);

    void attach(std::unique_ptr<IMedia>&& attachedMedia) override;
    void start() override;
    void stop() override;

    void setInTransition(std::unique_ptr<TransitionExecutor>&& transition) override;
    void setOutTransition(std::unique_ptr<TransitionExecutor>&& transition) override;

    SignalMediaFinished& mediaFinished() override;

    MediaGeometry::Align align() const override;
    MediaGeometry::Valign valign() const override;
    std::shared_ptr<IWidget> view() override;

protected:
    virtual void onStarted();
    virtual void onStopped();

private:
    void startTimer(int duration);
    void startAttachedMedia();
    void stopAttachedMedia();

    void applyInTransition();
    void applyOutTransition();

private:
    MediaOptions options_;
    std::shared_ptr<IWidget> view_;
    std::unique_ptr<IMedia> attachedMedia_;
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<TransitionExecutor> inTransition_;
    std::unique_ptr<TransitionExecutor> outTransition_;
    SignalMediaFinished mediaFinished_;
};
