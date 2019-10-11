#pragma once

#include "common/PlayerRuntimeError.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"
#include "control/media/player/gst/GstFwd.hpp"

#include <functional>

namespace ph = std::placeholders;

class GstMediaPlayer : public Xibo::MediaPlayer
{
public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    template <typename Player>
    static std::unique_ptr<GstMediaPlayer> create(MediaPlayerOptions::Location location)
    {
        auto player = std::make_unique<Player>();
        player->init(location);
        return player;
    }

    void load(const Uri& uri) override;
    void setVolume(int volume) override;
    void play() override;
    void stop() override;
    SignalPlaybackFinished& playbackFinished() override;

    void showOutputWindow() override;
    void hideOutputWindow() override;
    void setOutputWindow(const std::shared_ptr<Xibo::OutputWindow>& outputWindow) override;
    const std::shared_ptr<Xibo::OutputWindow>& outputWindow() const override;

protected:
    GstMediaPlayer() = default;

    virtual void createPipeline() = 0;

private:
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void createBaseElements(MediaPlayerOptions::Location location);
    void noMorePads();
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void init(MediaPlayerOptions::Location location);
    void checkVolume(int volume);

protected:
    Gst::RefPtr<Gst::Pipeline> pipeline_;
    Gst::RefPtr<Gst::UriSrc> source_;
    Gst::RefPtr<Gst::Decodebin> decodebin_;
    Gst::RefPtr<Gst::Volume> volume_;
    Gst::RefPtr<Gst::VideoConvert> videoConverter_;
    Gst::RefPtr<Gst::VideoScale> videoScale_;
    Gst::RefPtr<Gst::Element> videoSink_;
    Gst::RefPtr<Gst::Queue> queue_;
    Gst::RefPtr<Gst::Capsfilter> capsfilter_;
    Gst::RefPtr<Gst::AudioConvert> audioConverter_;
    Gst::RefPtr<Gst::AutoAudioSink> audioSink_;

    std::shared_ptr<Xibo::OutputWindow> outputWindow_;
    SignalPlaybackFinished playbackFinished_;
};
