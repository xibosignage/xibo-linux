#pragma once

#include "common/PlayerRuntimeError.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

#include <functional>
#include <gst/gstelement.h>

namespace ph = std::placeholders;

class GstMediaPlayer : public Xibo::MediaPlayer
{
public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    GstMediaPlayer();
    ~GstMediaPlayer() override;

    void load(const Uri& uri) override;
    void setVolume(int volume) override;
    void play() override;
    void stop() override;
    SignalPlaybackFinished& playbackFinished() override;

    void showOutputWindow() override;
    void hideOutputWindow() override;
    void setOutputWindow(const std::shared_ptr<Xibo::OutputWindow>& outputWindow) override;
    const std::shared_ptr<Xibo::OutputWindow>& outputWindow() const override;

private:
    static gboolean busMessageWatch(GstBus* bus, GstMessage* msg, gpointer player);
    void checkVolume(int volume);

protected:
    GstElement* playbin_;
    GstElement* videoSink_;
    GstBus* playbinBus_;

    std::shared_ptr<Xibo::OutputWindow> outputWindow_;
    SignalPlaybackFinished playbackFinished_;
};
