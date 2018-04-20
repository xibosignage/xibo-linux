#include "Queue.hpp"

Gst::Queue::Queue()
{
    m_element = gst_element_factory_make("queue", nullptr);
}

Gst::Queue* Gst::Queue::create()
{
    return new Gst::Queue;
}

void Gst::Queue::set_max_size_buffers(int max_size_buffers)
{
    g_object_set(m_element, "max-size-buffers", max_size_buffers, nullptr);
}
