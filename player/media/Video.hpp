#pragma once

#include "Media.hpp"

#include <gtkmm/drawingarea.h>
#include <gst/gst.h>
#include <spdlog/spdlog.h>

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
    gboolean bus_message_watch(GstBus* bus, GstMessage* message, gpointer user_data);
    void no_more_pads(GstElement* decodebin, gpointer user_data);
    void on_pad_added(GstElement* decodebin, GstPad* pad, gpointer user_data);

private:
    bool m_muted;
    bool m_looped;

    Gtk::DrawingArea m_video_window;
    guint m_watch_id;
    bool m_video_ended = false;
    std::shared_ptr<spdlog::logger> m_logger;

    GstElement* m_pipeline = nullptr;
    GstElement* m_source = nullptr;
    GstElement* m_decodebin = nullptr;
    GstElement* m_volume = nullptr;
    GstElement* m_video_converter = nullptr;
    GstElement* m_video_scale = nullptr;
    GstElement* m_video_sink = nullptr;
    GstElement* m_audio_converter = nullptr;
    GstElement* m_audio_sink = nullptr;
    GstElement* m_queue = nullptr;
};
