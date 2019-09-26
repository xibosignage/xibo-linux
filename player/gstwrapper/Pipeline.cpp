#include "Pipeline.hpp"

Gst::Pipeline::Pipeline() : Gst::Element(gst_pipeline_new("pipeline")) {}

gboolean Gst::Pipeline::onBusWatchMem(GstBus*, GstMessage* message, gpointer)
{
    return m_watchHandler(std::make_shared<Gst::Message>(message, false));
}

gboolean Gst::Pipeline::onBusWatch(GstBus* bus, GstMessage* message, gpointer data)
{
    return reinterpret_cast<Pipeline*>(data)->onBusWatchMem(bus, message, data);
}

Gst::Pipeline::~Pipeline()
{
    g_source_remove(m_watchId);
}

Gst::RefPtr<Gst::Pipeline> Gst::Pipeline::create()
{
    return Gst::RefPtr<Gst::Pipeline>(new Gst::Pipeline{});
}

Gst::RefPtr<Gst::Pipeline> Gst::Pipeline::add(Gst::RefPtr<Gst::Element> other)
{
    gst_bin_add(GST_BIN(element()), other->handler());
    return shared_from_this();
}

Gst::RefPtr<Gst::Pipeline> Gst::Pipeline::remove(Gst::RefPtr<Gst::Element> other)
{
    gst_bin_remove(GST_BIN(element()), other->handler());
    other->resetHandler();
    return shared_from_this();
}

void Gst::Pipeline::addBusWatch(std::function<bool(const Gst::RefPtr<Gst::Message>&)> handler)
{
    m_watchHandler = handler;

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(element()));

    m_watchId = gst_bus_add_watch(bus, onBusWatch, this);
    g_object_unref(bus);
}
