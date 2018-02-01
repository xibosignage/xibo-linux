#pragma once

#include <gst/gst.h>
#include <gtkmm/bin.h>
#include <gtkmm/drawingarea.h>
#include <spdlog/spdlog.h>

#include <gstreamermm/pipeline.h>
#include <gstreamermm/bus.h>
#include "constants.hpp"

#include <gstreamermm/filesrc.h>
#include <gstreamermm/decodebin.h>
#include <gstreamermm/volume.h>
#include <gstreamermm/videoconvert.h>
#include <gstreamermm/videosink.h>
#include <gstreamermm/audioconvert.h>
#include <gstreamermm/audiosink.h>

class VideoHandler : public Gtk::Bin
{
public:
    VideoHandler(const std::string& filename, const Size& size);
    ~VideoHandler();

    void set_volume(double volume);
    void set_size(int width, int height);
    void play();
    sigc::signal<void>& signal_video_ended();

private:
    void create_ui();
    void realize_cb();
    void update_video_size();

    Gst::BusSyncReply bus_sync_handler(const Glib::RefPtr<Gst::Bus>&, const Glib::RefPtr<Gst::Message>& message);
    bool bus_message_watch(const Glib::RefPtr<Gst::Bus>&, const Glib::RefPtr<Gst::Message>& message);
    void no_more_pads();
    void on_pad_added(const Glib::RefPtr<Gst::Pad>& pad);

private:
    Gtk::DrawingArea m_video_window;
    guintptr m_window_handle;
    guint m_watch_id;
    Size m_size, m_best_size;

    Glib::RefPtr<Gst::Pipeline> m_pipeline;
    Glib::RefPtr<Gst::FileSrc> m_source;
    Glib::RefPtr<Gst::DecodeBin> m_decodebin;
    Glib::RefPtr<Gst::Volume> m_volume;
    Glib::RefPtr<Gst::VideoConvert> m_video_converter;
    Glib::RefPtr<Gst::Element> m_video_sink;
    Glib::RefPtr<Gst::AudioConvert> m_audio_converter;
    Glib::RefPtr<Gst::Element> m_audio_sink;

    sigc::signal<void> m_signal_video_ended;
    bool m_video_ended = false;

    std::shared_ptr<spdlog::logger> m_logger;

};
