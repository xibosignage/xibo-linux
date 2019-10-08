#pragma once

#include "common/dt/Timer.hpp"
#include "control/media/Media.hpp"
#include "control/media/MediaOptions.hpp"
#include "control/transitions/TransitionExecutor.hpp"
#include "control/widgets/Widget.hpp"

class MediaImpl : public Xibo::Media
{
public:
    MediaImpl(const MediaOptions& options);

    void setWidget(const std::shared_ptr<Xibo::Widget>& widget) override;
    void attach(std::unique_ptr<Xibo::Media>&& attachedMedia) override;
    void start() override;
    void stop() override;

    void setInTransition(std::unique_ptr<TransitionExecutor>&& transition) override;
    void setOutTransition(std::unique_ptr<TransitionExecutor>&& transition) override;

    SignalMediaFinished& mediaFinished() override;

    MediaGeometry::Align align() const override;
    MediaGeometry::Valign valign() const override;
    std::shared_ptr<Xibo::Widget> view() override;

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
    std::shared_ptr<Xibo::Widget> widget_;
    std::unique_ptr<Xibo::Media> attachedMedia_;
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<TransitionExecutor> inTransition_;
    std::unique_ptr<TransitionExecutor> outTransition_;
    SignalMediaFinished mediaFinished_;
};
