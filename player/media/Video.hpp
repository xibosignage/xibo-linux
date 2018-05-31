#pragma once

#include "Media.hpp"

#include <gtkmm/drawingarea.h>
#include <gst/gst.h>
#include <spdlog/spdlog.h>

#include "wrapper/Pipeline.hpp"
#include "wrapper/VideoConvert.hpp"
#include "wrapper/AudioConvert.hpp"
#include "wrapper/Volume.hpp"
#include "wrapper/VideoScale.hpp"
#include "wrapper/Queue.hpp"
#include "wrapper/Decodebin.hpp"
#include "wrapper/FileSrc.hpp"
#include "wrapper/AutoAudioSink.hpp"
#include "wrapper/Element.hpp"
#include "wrapper/Pad.hpp"
#include "wrapper/Caps.hpp"

class XiboVideoSink;

class Video : public Media
{
public:
    Video(int id, const Size& size, int duration, const std::string& uri, bool muted, bool looped);
    ~Video() override;

    void stop() override;
    void start() override;
    void start_timer() override;
    void set_region(Region* region) override;

    void set_volume(double volume);
    void play();

private:
    bool bus_message_watch(const Gst::RefPtr<Gst::Message>& message);
    void no_more_pads();
    void on_pad_added(const Gst::RefPtr<Gst::Pad>& pad);

private:
    bool m_muted;
    bool m_looped;

    Gtk::DrawingArea m_video_window;
    guint m_watch_id;
    bool m_video_ended = false;
    std::shared_ptr<spdlog::logger> m_logger;

    Gst::RefPtr<Gst::Pipeline> m_pipeline;
    Gst::RefPtr<Gst::FileSrc> m_source;
    Gst::RefPtr<Gst::Decodebin> m_decodebin;
    Gst::RefPtr<Gst::Volume> m_volume;
    Gst::RefPtr<Gst::VideoConvert> m_video_converter;
    Gst::RefPtr<Gst::VideoScale> m_video_scale;
    Gst::RefPtr<Gst::Element> m_video_sink;
    Gst::RefPtr<Gst::AudioConvert> m_audio_converter;
    Gst::RefPtr<Gst::AutoAudioSink> m_audio_sink;
    Gst::RefPtr<Gst::Queue> m_queue;
};
