#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Element.hpp"
#include "Message.hpp"

#include <gst/gstpipeline.h>
#include <functional>

namespace Gst
{
    class Pipeline : public Element
    {
    public:
        ~Pipeline();
        static Pipeline* create();
        static Pipeline* create(const std::string& name);
        Pipeline* add(Element* other);
        Pipeline* remove(Element* other);
        void add_bus_watch(std::function<bool(const Message&)> handler);

    private:
        Pipeline(const std::string& name);
        gboolean on_bus_watch(GstBus*, GstMessage* message, gpointer);

    private:
        std::function<bool(const Message&)> m_watch_handler;
        guint m_watch_id;

    };
}

#endif // PIPELINE_HPP
