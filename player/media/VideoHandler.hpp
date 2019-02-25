#pragma once

#include "IVideoHandler.hpp"
#include "wrapper/GstFwd.hpp"

#include <sigc++/signal.h>

using SignalVideoFinished = sigc::signal<void()>;
class GtkDrawingAreaAdaptor;

class VideoHandler : public IVideoHandler
{
public:
    VideoHandler();
    ~VideoHandler() override;

    IWidgetAdaptor& videoWindow() override;

    void scale(double, double) override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

    void load(const Uri& uri) override;
    void play() override;
    void stopPlayback() override;
    void stop() override;
    void connect(OnVideoFinished callback) override;
    void setVolume(int volume) override;

private:
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void noMorePads();
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void init();
    void checkGstElements();
    void inspectVideo(const Uri& uri);

private:
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

    std::unique_ptr<GtkDrawingAreaAdaptor> m_videoWindow;
    SignalVideoFinished m_videoFinished;

};
