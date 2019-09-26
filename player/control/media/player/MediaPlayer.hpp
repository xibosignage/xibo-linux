#pragma once

#include "IMediaPlayer.hpp"
#include "gstwrapper/GstFwd.hpp"
#include "video/VideoWindow.hpp"

#include <functional>

class MediaPlayer : public IMediaPlayer
{
public:
    MediaPlayer();
    ~MediaPlayer() override;

    void setOutputWindow(const std::shared_ptr<IVideoWindow>& window) override;
    std::shared_ptr<IVideoWindow> outputWindow() const override;

    void load(const Uri& uri) override;
    void setVolume(int volume) override;

    void play() override;
    void stopAndRemove() override;
    void stopPlayback() override;

    SignalPlaybackFinished& playbackFinished() override;
    Gst::InspectorResult mediaInfo() const override;

private:
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void noMorePads();
    void inspectFile(const Uri& uri);
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void init();
    void createGstElements();

private:
    std::shared_ptr<IVideoWindow> outputWindow_;

    Gst::RefPtr<Gst::Pipeline> pipeline_;
    Gst::RefPtr<Gst::UriSrc> source_;
    Gst::RefPtr<Gst::Decodebin> decodebin_;
    Gst::RefPtr<Gst::Volume> volume_;
    Gst::RefPtr<Gst::VideoConvert> videoConverter_;
    Gst::RefPtr<Gst::VideoScale> videoScale_;
    Gst::RefPtr<Gst::Element> videoSink_;
    Gst::RefPtr<Gst::AudioConvert> audioConverter_;
    Gst::RefPtr<Gst::AutoAudioSink> audioSink_;
    Gst::RefPtr<Gst::Queue> queue_;
    Gst::RefPtr<Gst::Capsfilter> capsfilter_;
    Gst::RefPtr<Gst::Inspector> inspector_;
    Gst::InspectorResult mediaInfo_;
    SignalPlaybackFinished playbackFinished_;
};
