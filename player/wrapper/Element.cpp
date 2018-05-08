#include "Element.hpp"
#include "Pad.hpp"

Gst::Element::Element(const std::string& name)
{
    m_element = gst_element_factory_make(name.c_str(), nullptr);
}

Gst::Element::~Element()
{
    if(m_element)
    {
        GstObject* parent = gst_element_get_parent(m_element);
        if(!parent)
        {
            set_state(Gst::State::NULL_STATE);
            g_object_unref(m_element);
        }
        else
        {
            g_object_unref(parent);
        }
    }
}

Gst::RefPtr<Gst::Element> Gst::Element::link(const Gst::RefPtr<Gst::Element>& other)
{
    if(m_element)
    {
        gst_element_link(m_element, other->get_handler());
        return other;
    }
    return nullptr;
}

void Gst::Element::set_state(Gst::State state)
{
    if(m_element)
    {
        gst_element_set_state(m_element, static_cast<GstState>(state));
    }
}

Gst::RefPtr<Gst::Pad> Gst::Element::get_static_pad(const std::string& name)
{
    if(m_element)
    {
        auto pad = gst_element_get_static_pad(m_element, name.c_str());

        if(!pad)
            return nullptr;
        return std::make_shared<Gst::Pad>(pad);
    }
    return nullptr;
}

Gst::RefPtr<Gst::Element> Gst::Element::create(const std::string& name)
{
    return std::shared_ptr<Gst::Element>(new Gst::Element(name));
}

bool Gst::Element::seek(gdouble rate, Gst::Format format, Gst::SeekFlags flags, Gst::SeekType start_type, gint64 start, Gst::SeekType stop_type, gint64 stop)
{
    if(m_element)
    {
        return gst_element_seek(m_element, rate, static_cast<GstFormat>(format), static_cast<GstSeekFlags>(flags),
                                static_cast<GstSeekType>(start_type), start, static_cast<GstSeekType>(stop_type), stop);
    }
    return false;
}

GstElement* Gst::Element::get_handler() const
{
    return m_element;
}

void Gst::Element::reset_handler()
{
    m_element = nullptr;
}
