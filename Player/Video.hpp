#pragma once

#include "Media.hpp"

#include <gtkmm/bin.h>
#include <gtkmm/drawingarea.h>
#include <gst/gst.h>
#include <spdlog/spdlog.h>

#include "constants.hpp"

class XiboVideoSink;

class Video : public Media
{
public:
    Video(const Region& region, int id, int duration, const std::string& uri, bool muted, bool looped);
    ~Video() override;

    void stop() override;
    void start() override;

    void set_volume(double volume);
    void set_size(int width, int height);
    void play();
    sigc::signal<void>& signal_video_ended();

private:
    void create_ui();
    void realize_cb();

    gboolean bus_message_watch(GstBus* bus, GstMessage* message, gpointer user_data);
    void no_more_pads(GstElement* decodebin, gpointer user_data);
    void on_pad_added(GstElement* decodebin, GstPad* pad, gpointer user_data);

private:
    bool m_muted;
    bool m_looped;

    Gtk::DrawingArea m_video_window;
    guintptr m_window_handle;
    guint m_watch_id;
    Size m_size, m_best_size;

    GstElement* m_pipeline;
    GstElement* m_source;
    GstElement* m_decodebin;
    GstElement* m_volume;
    GstElement* m_video_converter;
    GstElement* m_video_sink;
    GstElement* m_audio_converter;
    GstElement* m_audio_sink;
    GstElement* m_queue;

    bool m_video_ended = false;

    std::shared_ptr<spdlog::logger> m_logger;
};
