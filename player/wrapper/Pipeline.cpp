#include "Pipeline.hpp"
#include "utils/BindWrapper.hpp"

namespace ph = std::placeholders;

Gst::Pipeline::Pipeline(const std::string& name)
{
    m_element = gst_pipeline_new(name.c_str());
}

gboolean Gst::Pipeline::on_bus_watch(GstBus*, GstMessage* message, gpointer)
{
    return m_watch_handler(Gst::Message(message));
}

Gst::Pipeline::~Pipeline()
{
    g_source_remove(m_watch_id);
}

Gst::Pipeline* Gst::Pipeline::create()
{
    return new Pipeline(std::string{});
}

Gst::Pipeline* Gst::Pipeline::create(const std::string& name)
{
    return new Pipeline(name);
}

Gst::Pipeline* Gst::Pipeline::add(Element* other)
{
    gst_bin_add(GST_BIN(m_element), other->get_handler());
    return this;
}

Gst::Pipeline* Gst::Pipeline::remove(Gst::Element* other)
{
    gst_bin_remove(GST_BIN(m_element), other->get_handler());
    return this;
}

void Gst::Pipeline::add_bus_watch(std::function<bool(const Message&)> handler)
{
    m_watch_handler = handler;

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(m_element));
    auto on_bus_watch = get_wrapper<2, gboolean, GstBus*, GstMessage*, gpointer>(std::bind(&Pipeline::on_bus_watch, this, ph::_1, ph::_2, ph::_3));

    m_watch_id = gst_bus_add_watch(bus, on_bus_watch, nullptr);
    g_object_unref(bus);
}
