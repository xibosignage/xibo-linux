#pragma once

#include "Element.hpp"
#include "Message.hpp"

#include <functional>
#include <gst/gstpipeline.h>
#include <set>

namespace Gst
{
class Pipeline : public Gst::Element, public std::enable_shared_from_this<Pipeline>
{
public:
    ~Pipeline();
    static Gst::RefPtr<Gst::Pipeline> create();
    Gst::RefPtr<Gst::Pipeline> add(Gst::RefPtr<Gst::Element> other);
    Gst::RefPtr<Gst::Pipeline> remove(Gst::RefPtr<Gst::Element> other);
    void addBusWatch(std::function<bool(const Gst::RefPtr<Gst::Message>&)> handler);

private:
    Pipeline();
    gboolean onBusWatchMem(GstBus*, GstMessage* message, gpointer);
    static gboolean onBusWatch(GstBus*, GstMessage* message, gpointer);

private:
    std::function<bool(const Gst::RefPtr<Gst::Message>&)> m_watchHandler;
    guint m_watchId;
};
}
