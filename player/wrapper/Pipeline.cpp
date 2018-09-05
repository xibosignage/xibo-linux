#include "Pipeline.hpp"
#include "utils/BindWrapper.hpp"

namespace ph = std::placeholders;

Gst::Pipeline::Pipeline(const std::string& name)
{
    setElement(gst_pipeline_new(name.c_str()));
}

gboolean Gst::Pipeline::onBusWatch(GstBus*, GstMessage* message, gpointer)
{
    return m_watchHandler(std::make_shared<Gst::Message>(message, false));
}

Gst::Pipeline::~Pipeline()
{
    g_source_remove(m_watchId);
}

Gst::RefPtr<Gst::Pipeline> Gst::Pipeline::create()
{
    return std::shared_ptr<Gst::Pipeline>(new Gst::Pipeline(std::string{}));
}

Gst::RefPtr<Gst::Pipeline> Gst::Pipeline::create(const std::string& name)
{
    return std::shared_ptr<Gst::Pipeline>(new Gst::Pipeline(name));
}

Gst::RefPtr<Gst::Pipeline> Gst::Pipeline::add(Gst::RefPtr<Gst::Element> other)
{
    gst_bin_add(GST_BIN(element()), other->getHandler());
    return shared_from_this();
}

Gst::RefPtr<Gst::Pipeline> Gst::Pipeline::remove(Gst::RefPtr<Gst::Element> other)
{
    gst_bin_remove(GST_BIN(element()), other->getHandler());
    other->resetHandler();
    return shared_from_this();
}

void Gst::Pipeline::addBusWatch(std::function<bool(const Gst::RefPtr<Gst::Message>&)> handler)
{
    m_watchHandler = handler;

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(element()));
    auto onBusWatch = get_wrapper<2, gboolean, GstBus*, GstMessage*, gpointer>(std::bind(&Pipeline::onBusWatch, this, ph::_1, ph::_2, ph::_3));

    m_watchId = gst_bus_add_watch(bus, onBusWatch, nullptr);
    g_object_unref(bus);
}
