#pragma once

#include "Media.hpp"
#include "wrapper/GstFwd.hpp"

#include <gtkmm/drawingarea.h>
#include <spdlog/spdlog.h>
#include <boost/format.hpp>

class XiboVideoSink;

class Video : public Media
{
public:
    Video(int width, int height, int duration, const std::string& uri, bool muted, bool looped);
    ~Video() override;

    void startTimer() override;
    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;

    bool muted() const;
    bool looped() const;

protected:
    void doStop() override;
    void doStart() override;
    void setSize(int, int) override;
    int width() const override { }
    int height() const override { }

private:
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void noMorePads();
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void setVolume(double volume);
    void play();

private:
    bool m_muted;
    bool m_looped;

    std::shared_ptr<spdlog::logger> m_logger;
    Gtk::DrawingArea m_videoWindow;
    bool m_videoEnded = false;
    boost::format m_videoFmt;

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
};
