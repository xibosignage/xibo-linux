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

class XiboVideoSink;

class Video : public Media
{
public:
    Video(const Region& region, int id, int duration, const std::string& uri, bool muted, bool looped);
    ~Video() override;

    void stop() override;
    void start() override;
    void start_timer() override;

    void set_volume(double volume);
    void play();

private:
    bool bus_message_watch(Gst::Message* message);
    void no_more_pads();
    void on_pad_added(Gst::Pad* pad);

private:
    bool m_muted;
    bool m_looped;

    Gtk::DrawingArea m_video_window;
    guint m_watch_id;
    bool m_video_ended = false;
    std::shared_ptr<spdlog::logger> m_logger;

    Gst::Pipeline* m_pipeline = nullptr;
    Gst::FileSrc* m_source = nullptr;
    Gst::Decodebin* m_decodebin = nullptr;
    Gst::Volume* m_volume = nullptr;
    Gst::VideoConvert* m_video_converter = nullptr;
    Gst::VideoScale* m_video_scale = nullptr;
    Gst::Element* m_video_sink = nullptr;
    Gst::AudioConvert* m_audio_converter = nullptr;
    Gst::AutoAudioSink* m_audio_sink = nullptr;
    Gst::Queue* m_queue = nullptr;
};
