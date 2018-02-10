#pragma once

#include <gtkmm.h>
#include <gstreamermm.h>
#include <gstreamermm/private/videosink_p.h>

#include <cassert>

class XiboVideoSink : public Gst::VideoSink
{
public:
    explicit XiboVideoSink(GstVideoSink* gobj);

    static void class_init(Gst::ElementClass<XiboVideoSink>* klass);
    static bool register_sink(Glib::RefPtr<Gst::Plugin> plugin);

    void set_handler(Gtk::DrawingArea* area);

    bool set_caps_vfunc(const Glib::RefPtr<Gst::Caps>& caps) override;
    Gst::FlowReturn render_vfunc(const Glib::RefPtr<Gst::Buffer>& buffer) override;

private:
    bool on_frame_drawn(const Cairo::RefPtr<::Cairo::Context>& cairo);

private:
    Glib::RefPtr<Gst::Pad> m_sinkpad;
    Gst::VideoInfo m_info;
    Gtk::DrawingArea* m_handler;
    Cairo::RefPtr<Cairo::ImageSurface> m_surface;
};
