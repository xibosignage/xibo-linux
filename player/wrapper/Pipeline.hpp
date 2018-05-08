#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Element.hpp"
#include "Message.hpp"

#include <gst/gstpipeline.h>
#include <functional>
#include <set>

namespace Gst
{
    class Pipeline : public Gst::Element, public std::enable_shared_from_this<Pipeline>
    {
    public:
        ~Pipeline();
        static Gst::RefPtr<Gst::Pipeline> create();
        static Gst::RefPtr<Gst::Pipeline> create(const std::string& name);
        Gst::RefPtr<Gst::Pipeline> add(Gst::RefPtr<Gst::Element> other);
        Gst::RefPtr<Gst::Pipeline> remove(Gst::RefPtr<Gst::Element> other);
        void add_bus_watch(std::function<bool(const Gst::RefPtr<Gst::Message>&)> handler);

    private:
        Pipeline(const std::string& name);
        gboolean on_bus_watch(GstBus*, GstMessage* message, gpointer);

    private:
        std::function<bool(const Gst::RefPtr<Gst::Message>&)> m_watch_handler;
        guint m_watch_id;

    };
}

#endif // PIPELINE_HPP
