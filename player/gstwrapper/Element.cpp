#include "Element.hpp"
#include "Pad.hpp"

Gst::Element::Element(std::string_view name)
{
    m_element = gst_element_factory_make(name.data(), nullptr);
}

Gst::Element::Element(GstElement* element) : m_element(element) {}

Gst::Element::~Element()
{
    if (m_element)
    {
        GstObject* parent = gst_element_get_parent(m_element);
        if (!parent)
        {
            setState(Gst::State::NULL_STATE);
            g_object_unref(m_element);
        }
        else
        {
            g_object_unref(parent);
        }
    }
}

GstElement* Gst::Element::element() const
{
    return m_element;
}

Gst::RefPtr<Gst::Element> Gst::Element::link(const Gst::RefPtr<Gst::Element>& other)
{
    if (m_element)
    {
        gst_element_link(m_element, other->handler());
        return other;
    }
    return nullptr;
}

Gst::RefPtr<Gst::Element> Gst::Element::linkFltered(const Gst::RefPtr<Gst::Element>& other, GstCaps* filter)
{
    if (m_element)
    {
        gst_element_link_filtered(m_element, other->handler(), filter);
        return other;
    }
    return nullptr;
}

void Gst::Element::setState(Gst::State state)
{
    if (m_element)
    {
        gst_element_set_state(m_element, static_cast<GstState>(state));
    }
}

Gst::State Gst::Element::state() const
{
    GstState state;
    gst_element_get_state(m_element, &state, nullptr, GST_MSECOND);
    return static_cast<Gst::State>(state);
}

Gst::RefPtr<Gst::Pad> Gst::Element::staticPad(std::string_view name)
{
    if (m_element)
    {
        auto pad = gst_element_get_static_pad(m_element, name.data());

        if (!pad) return nullptr;
        return std::make_shared<Gst::Pad>(pad);
    }
    return nullptr;
}

Gst::RefPtr<Gst::Element> Gst::Element::create(std::string_view name)
{
    return Gst::RefPtr<Gst::Element>(new Gst::Element(name));
}

bool Gst::Element::seek(gdouble rate, Gst::Format format, Gst::SeekFlags flags, Gst::SeekType startType, gint64 start,
                        Gst::SeekType stopType, gint64 stop)
{
    if (m_element)
    {
        return gst_element_seek(m_element, rate, static_cast<GstFormat>(format), static_cast<GstSeekFlags>(flags),
                                static_cast<GstSeekType>(startType), start, static_cast<GstSeekType>(stopType), stop);
    }
    return false;
}

GstElement* Gst::Element::handler() const
{
    return m_element;
}

void Gst::Element::resetHandler()
{
    m_element = nullptr;
}
