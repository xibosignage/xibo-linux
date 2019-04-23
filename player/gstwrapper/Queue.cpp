#include "Queue.hpp"

Gst::Queue::Queue() :
    Gst::Element("queue")
{
}

Gst::RefPtr<Gst::Queue> Gst::Queue::create()
{
    return std::shared_ptr<Gst::Queue>(new Gst::Queue);
}

void Gst::Queue::setMaxSizeBuffers(int maxSizeBuffers)
{
    g_object_set(element(), "max-size-buffers", maxSizeBuffers, nullptr);
}
