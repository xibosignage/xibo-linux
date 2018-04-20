#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Element.hpp"
#include "Message.hpp"

#include <gst/gstpipeline.h>
#include <functional>
#include <set>

namespace Gst
{
    class Pipeline : public Gst::Element
    {
    public:
        ~Pipeline();
        static Pipeline* create();
        static Pipeline* create(const std::string& name);
        Gst::Pipeline* add(Gst::Element* other);
        Gst::Pipeline* remove(Gst::Element* other);
        void add_bus_watch(std::function<bool(Gst::Message*)> handler);

    private:
        Pipeline(const std::string& name);
        gboolean on_bus_watch(GstBus*, GstMessage* message, gpointer);

    private:
        std::function<bool(Gst::Message*)> m_watch_handler;
        std::set<Gst::Element*> m_bin;
        guint m_watch_id;

    };
}

#endif // PIPELINE_HPP
