#pragma once

#include <gtkmm.h>
#include <gstreamermm.h>
#include <gstreamermm/private/videosink_p.h>

#include <cassert>

class CustomVideoSink : public Gst::VideoSink
{
public:
    explicit CustomVideoSink(GstVideoSink* gobj);

    static void class_init(Gst::ElementClass<CustomVideoSink>* klass);
    static bool register_sink(Glib::RefPtr<Gst::Plugin> plugin);

    bool set_caps_vfunc(const Glib::RefPtr<Gst::Caps>& caps) override;
    Gst::FlowReturn render_vfunc(const Glib::RefPtr<Gst::Buffer>& buffer) override;

private:
    Glib::RefPtr<Gst::Pad> sinkpad;
    Gst::VideoInfo info;
    Glib::Property<Gtk::DrawingArea*> area_property;
    Cairo::RefPtr<Cairo::ImageSurface> surface;
    sigc::connection m_connection;
};
