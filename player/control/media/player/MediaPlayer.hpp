#pragma once

#include "gstwrapper/GstFwd.hpp"
#include "gstwrapper/Inspector.hpp"
#include "video/VideoWindow.hpp"

#include <functional>
#include <sigc++/signal.h>

class Uri;
class Widget;
using SignalPlaybackFinished = sigc::signal<void()>;

class MediaPlayer
{
public:
    MediaPlayer(Widget& outputWindow);
    MediaPlayer();
    ~MediaPlayer();

    void setOutputWindow(const std::shared_ptr<VideoWindow>& window);
    std::shared_ptr<VideoWindow> outputWindow() const;

    void load(const Uri& uri);
    void setVolume(int volume);

    void play();
    void stopAndRemove();
    void stopPlayback();

    SignalPlaybackFinished playbackFinished();
    Gst::InspectorResult mediaInfo() const;

private:
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void noMorePads();
    void inspectFile(const Uri& uri);
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void init();
    void createGstElements();

private:
    std::shared_ptr<VideoWindow> m_outputWindow;

    Gst::RefPtr<Gst::Pipeline> m_pipeline;
    Gst::RefPtr<Gst::FileSrc> m_source;
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
