#pragma once

// remove
#include "Media.hpp"

#include <gst/gst.h>
#include <gtkmm/bin.h>
#include <gtkmm/drawingarea.h>

struct Converter
{
    GstElement* video;
    GstElement* audio;
};

class VideoHandler : public Gtk::Bin
{
public:
    VideoHandler(const std::string& filename, const Size& size);
    ~VideoHandler();

    void set_volume(double volume);
    void set_size(const Size& size);
    void play();

private:
    void create_ui();
    void realize_cb();

    GstBusSyncReply bus_sync_handler(GstBus* bus, GstMessage* message, gpointer data);
    gboolean cb_message_error(GstBus* bus, GstMessage* msg, gpointer data);
    gboolean cb_message_eos(GstBus* bus, GstMessage* msg, gpointer data);
    void no_more_pads(GstElement* element, gpointer data);
    void on_pad_added(GstElement* element, GstPad* pad, gpointer data);

private:
    Gtk::DrawingArea video_window;
    Size m_size;    
    guintptr m_windowHandle;
    GstElement* m_pipeline;
    GstElement* m_source;
    GstElement* m_decodebin;
    GstElement* m_volume;
    GstElement* m_videoConverter;
    GstElement* m_videoSink;
    GstElement* m_audioConverter;
    GstElement* m_audioSink;
    Converter m_converter;

};
