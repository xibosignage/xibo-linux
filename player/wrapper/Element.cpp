#include "Element.hpp"

Gst::Element::Element()
{

}

Gst::Element::Element(const std::string& name)
{
    m_element = gst_element_factory_make(name.c_str(), nullptr);
}

Gst::Element::~Element()
{
    set_state(Gst::State::NULL_STATE);
    g_object_unref(m_element);
}

Gst::Element* Gst::Element::link(Element* other)
{
    gst_element_link(m_element, other->get_handler());
    return other;
}

void Gst::Element::set_state(Gst::State state)
{
    gst_element_set_state(m_element, static_cast<GstState>(state));
}

Gst::Pad* Gst::Element::get_static_pad(const std::string& name)
{
    auto pad = gst_element_get_static_pad(m_element, name.c_str());

    if(!pad)
        return nullptr;
    return new Gst::Pad(pad);
}

Gst::Element* Gst::Element::create(const std::string& name)
{
    return new Gst::Element(name);
}

bool Gst::Element::seek(gdouble rate, Gst::Format format, Gst::SeekFlags flags, Gst::SeekType start_type, gint64 start, Gst::SeekType stop_type, gint64 stop)
{
    return gst_element_seek(m_element, rate, static_cast<GstFormat>(format), static_cast<GstSeekFlags>(flags),
                            static_cast<GstSeekType>(start_type), start, static_cast<GstSeekType>(stop_type), stop);
}

GstElement* Gst::Element::get_handler() const
{
    return m_element;
}
