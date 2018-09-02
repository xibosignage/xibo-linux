#include "Queue.hpp"

Gst::Queue::Queue()
{
    m_element = gst_element_factory_make("queue", nullptr);
}

Gst::RefPtr<Gst::Queue> Gst::Queue::create()
{
    return std::shared_ptr<Gst::Queue>(new Gst::Queue);
}

void Gst::Queue::setMaxSizeBuffers(int maxSizeBuffers)
{
    g_object_set(m_element, "max-size-buffers", maxSizeBuffers, nullptr);
}
