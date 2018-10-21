#pragma once

#include "IVideoHandler.hpp"
#include "GtkBaseAdaptor.hpp"
#include "wrapper/GstFwd.hpp"

#include <sigc++/signal.h>
#include <boost/format.hpp>
#include <gtkmm/drawingarea.h>

using SignalVideoFinished = sigc::signal<void()>;

class VideoHandler : public IVideoHandler, public GtkBaseAdaptor
{
public:
    VideoHandler();

    void show() override { }
    void hide() override { }
    bool isShown() const override { }

    void scale(double, double) override { }
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

    Gtk::Widget& get() override;

    void load(const FilePath& path) override;
    void play() override;
    void stop() override;
    void connect(OnVideoFinished callback) override;
    void setVolume(double volume) override;

private:
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void noMorePads();
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void init();
    void checkGstElements();

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

    Gtk::DrawingArea m_videoWindow;
    SignalVideoFinished m_videoFinished;
    boost::format m_videoFmt;

};
