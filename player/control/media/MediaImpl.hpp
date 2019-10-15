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

    void enableStat(bool enable) override;
    MediaOptions::StatPolicy statPolicy() const override;

    void inTransition(std::unique_ptr<TransitionExecutor>&& transition) override;
    void outTransition(std::unique_ptr<TransitionExecutor>&& transition) override;

    SignalMediaFinished& finished() override;
    SignalMediaStatReady& statReady() override;

    MediaGeometry::Align align() const override;
    MediaGeometry::Valign valign() const override;
    std::shared_ptr<Xibo::Widget> view() override;

protected:
    virtual void onStarted();
    virtual void onStopped();

private:
    void startTimer(int duration);
    void sendStat();

    void startAttachedMedia();
    void stopAttachedMedia();

    void applyInTransition();
    void applyOutTransition();

private:
    MediaOptions options_;
    std::unique_ptr<Timer> timer_;

    bool enableStat_;
    MediaStat stat_;

    std::shared_ptr<Xibo::Widget> widget_;
    std::unique_ptr<Xibo::Media> attachedMedia_;
    std::unique_ptr<TransitionExecutor> inTransition_;
    std::unique_ptr<TransitionExecutor> outTransition_;
    SignalMediaFinished finished_;
    SignalMediaStatReady statReady_;
};
