#include "Capsfilter.hpp"
#include "Caps.hpp"

Gst::Capsfilter::Capsfilter()
{
    m_element = gst_element_factory_make("capsfilter", nullptr);
}

Gst::RefPtr<Gst::Capsfilter> Gst::Capsfilter::create()
{
    return std::shared_ptr<Gst::Capsfilter>(new Gst::Capsfilter);
}

void Gst::Capsfilter::set_caps(Gst::RefPtr<Gst::Caps> caps)
{
    m_caps = std::move(caps);
    g_object_set(m_element, "caps", m_caps->get_handler(), nullptr);
}
