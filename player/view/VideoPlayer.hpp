#pragma once

#include "wrapper/GstFwd.hpp"
#include "events/EventPublisher.hpp"

#include <functional>

class Uri;
class DrawingArea;

class VideoPlayer : public EventPublisher<>
{
public:
    VideoPlayer(DrawingArea& window);
    ~VideoPlayer();

    void load(const Uri& uri);
    void play();
    void stop();
    void setVolume(int volume);

private:
    void setVideoSize(int width, int height);
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void noMorePads();
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void init();
    void checkGstElements();
    void inspectVideo(const Uri& uri);

private:
    DrawingArea& m_videoWindow;

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

};
