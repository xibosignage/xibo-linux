#include "XiboVideoSink.hpp"

#include <spdlog/spdlog.h>

XiboVideoSink::XiboVideoSink(GstVideoSink* gobj) :
    Glib::ObjectBase(typeid(XiboVideoSink)),
    Gst::VideoSink(gobj)
{
    m_info.init();
    add_pad(m_sinkpad = Gst::Pad::create(get_pad_template("sink"), "xibosink"));
}

void XiboVideoSink::class_init(Gst::ElementClass<XiboVideoSink>* klass)
{
    klass->set_metadata("Xibo Video Sink plugin", "Some classification", "Plugin to provide video playing on drawing area", "Stivius");
    std::string caps_str = "video/x-raw, format = (string) { BGRx }, width = (int) [ 1, max ], height = (int) [ 1, max ], framerate = (fraction) [ 0, max ]";
    klass->add_pad_template(Gst::PadTemplate::create("sink", Gst::PAD_SINK, Gst::PAD_ALWAYS, Gst::Caps::create_from_string(caps_str)));
}

bool XiboVideoSink::register_sink(Glib::RefPtr<Gst::Plugin> plugin)
{
    Gst::ElementFactory::register_element(plugin, "xibovideosink", 10, Gst::register_mm_type<XiboVideoSink>("videosink"));
    return true;
}

void XiboVideoSink::set_handler(Gtk::DrawingArea* handler)
{
    m_handler = handler;
    m_handler->signal_draw().connect(sigc::mem_fun(*this, &XiboVideoSink::on_frame_drawn));
}

bool XiboVideoSink::on_frame_drawn(const Cairo::RefPtr<::Cairo::Context>& cairo)
{
    if(m_surface)
    {
        double dx = m_handler->get_width() / static_cast<double>(m_info.get_width());
        double dy = m_handler->get_height() / static_cast<double>(m_info.get_height());
        dx = dy = std::min(dx, dy);

        cairo->scale(dx, dy);
        cairo->set_source(m_surface, 0, 0);
        cairo->paint();
    }
    return true;
}

bool XiboVideoSink::set_caps_vfunc(const Glib::RefPtr<Gst::Caps>& caps)
{
    m_info.from_caps(caps);
    return true;
}

Gst::FlowReturn XiboVideoSink::render_vfunc(const Glib::RefPtr<Gst::Buffer>& buffer)
{
    Gst::VideoFrame frame;
    if(frame.map(m_info, buffer, Gst::MapFlags::MAP_READ))
    {
        m_surface = Cairo::ImageSurface::create(static_cast<u_char*>(frame.gobj()->data[0]),
                                                Cairo::FORMAT_RGB24,
                                                m_info.get_width(),
                                                m_info.get_height(),
                                                frame.get_info().gobj()->stride[0]);
        m_handler->queue_draw();
        frame.unmap();
    }
    return Gst::FLOW_OK;
}
