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
    std::shared_ptr<IVideoWindow> m_outputWindow;

    Gst::RefPtr<Gst::Pipeline> m_pipeline;
    Gst::RefPtr<Gst::UriSrc> m_source;
    Gst::RefPtr<Gst::Decodebin> m_decodebin;
    Gst::RefPtr<Gst::Volume> m_volume;
    Gst::RefPtr<Gst::VideoConvert> m_videoConverter;
    Gst::RefPtr<Gst::VideoScale> m_videoScale;
    Gst::RefPtr<Gst::Element> m_videoSink;
    Gst::RefPtr<Gst::AudioConvert> m_audioConverter;
    Gst::RefPtr<Gst::AutoAudioSink> m_audioSink;
    Gst::RefPtr<Gst::Queue> m_queue;
    Gst::RefPtr<Gst::Capsfilter> m_capsfilter;
    Gst::RefPtr<Gst::Inspector> m_inspector;
    Gst::InspectorResult m_mediaInfo;
    SignalPlaybackFinished m_playbackFinished;
};
