#include "CustomVideoSink.hpp"

#include <spdlog/spdlog.h>
#include "constants.hpp"

CustomVideoSink::CustomVideoSink(GstVideoSink* gobj) :
    Glib::ObjectBase(typeid(CustomVideoSink)),
    Gst::VideoSink(gobj),
    area_property(*this, "area")
{
    info.init();
    add_pad(sinkpad = Gst::Pad::create(get_pad_template("sink"), "mysink"));
}

void CustomVideoSink::class_init(Gst::ElementClass<CustomVideoSink>* klass)
{
    klass->set_metadata("foo_longname", "foo_classification", "foo_detail_description", "foo_detail_author");
    std::string caps_str = "video/x-raw, format = (string) { BGRx }, width = (int) [ 1, max ], height = (int) [ 1, max ], framerate = (fraction) [ 0, max ]";
    klass->add_pad_template(Gst::PadTemplate::create("sink", Gst::PAD_SINK, Gst::PAD_ALWAYS, Gst::Caps::create_from_string(caps_str)));
}

bool CustomVideoSink::register_sink(Glib::RefPtr<Gst::Plugin> plugin)
{
    Gst::ElementFactory::register_element(plugin, "myvideosink", 10, Gst::register_mm_type<CustomVideoSink>("videosink"));
    return true;
}

bool CustomVideoSink::set_caps_vfunc(const Glib::RefPtr<Gst::Caps>& caps)
{
    info.from_caps(caps);
    return true;
}

#include <gst/gst.h>

Gst::FlowReturn CustomVideoSink::render_vfunc(const Glib::RefPtr<Gst::Buffer>& buffer)
{
    if(!m_connection)
    {
        spdlog::get(LOGGER)->debug("connection");

        m_connection = area_property.get_value()->signal_draw().connect([=](const ::Cairo::RefPtr<::Cairo::Context>& cairo){
            Gst::VideoRectangle src = {
                0,
                0,
                area_property.get_value()->get_width(),
                area_property.get_value()->get_height()
            };
            Gst::VideoRectangle dst = {
                0,
                0,
                info.get_width(),
                info.get_height()
            };
            double dx = area_property.get_value()->get_width() / (double)info.get_width();
            double dy = area_property.get_value()->get_height() / (double)info.get_height();

//            spdlog::get(LOGGER)->debug("{} {}", area_property.get_value()->get_width(), area_property.get_value()->get_height());

            dx = std::min(dx, dy);
            dy = dx;
            cairo->scale(dx, dy);
            cairo->set_source(surface, 0, 0);
            cairo->paint();
            return true;
        });
    }
    Gst::VideoFrame frame;
    if(frame.map(info, buffer, Gst::MapFlags::MAP_READ))
    {
        surface = Cairo::ImageSurface::create((u_char*)frame.gobj()->data[0], Cairo::FORMAT_RGB24,
                                              info.get_width(), info.get_height(), frame.get_info().gobj()->stride[0]);
        area_property.get_value()->queue_draw();
        frame.unmap();
    }
    else
    {
        spdlog::get(LOGGER)->debug("wrong mapping");
    }

    return Gst::FLOW_OK;
}
