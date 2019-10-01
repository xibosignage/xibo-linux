#include "Element.hpp"
#include "Pad.hpp"

Gst::Element::Element(std::string_view name)
{
    handler_ = gst_element_factory_make(name.data(), nullptr);
}

Gst::Element::Element(GstElement* element) : handler_(element) {}

Gst::Element::~Element()
{
    if (handler_)
    {
        GstObject* parent = gst_element_get_parent(handler_);
        if (!parent)
        {
            setState(Gst::State::NULL_STATE);
            g_object_unref(handler_);
        }
        else
        {
            g_object_unref(parent);
        }
    }
}

GstElement* Gst::Element::element() const
{
    return handler_;
}

Gst::RefPtr<Gst::Element> Gst::Element::link(const Gst::RefPtr<Gst::Element>& other)
{
    if (handler_)
    {
        gst_element_link(handler_, other->handler());
        return other;
    }
    return nullptr;
}

Gst::RefPtr<Gst::Element> Gst::Element::linkFltered(const Gst::RefPtr<Gst::Element>& other, GstCaps* filter)
{
    if (handler_)
    {
        gst_element_link_filtered(handler_, other->handler(), filter);
        return other;
    }
    return nullptr;
}

void Gst::Element::setState(Gst::State state)
{
    if (handler_)
    {
        gst_element_set_state(handler_, static_cast<GstState>(state));
    }
}

Gst::State Gst::Element::state() const
{
    GstState state;
    gst_element_get_state(handler_, &state, nullptr, GST_MSECOND);
    return static_cast<Gst::State>(state);
}

Gst::RefPtr<Gst::Pad> Gst::Element::staticPad(std::string_view name)
{
    if (handler_)
    {
        auto pad = gst_element_get_static_pad(handler_, name.data());

        if (!pad) return nullptr;
        return std::make_shared<Gst::Pad>(pad);
    }
    return nullptr;
}

Gst::RefPtr<Gst::Element> Gst::Element::create(std::string_view name)
{
    return Gst::RefPtr<Gst::Element>(new Gst::Element(name));
}

bool Gst::Element::seek(gdouble rate,
                        Gst::Format format,
                        Gst::SeekFlags flags,
                        Gst::SeekType startType,
                        gint64 start,
                        Gst::SeekType stopType,
                        gint64 stop)
{
    if (handler_)
    {
        return gst_element_seek(handler_,
                                rate,
                                static_cast<GstFormat>(format),
                                static_cast<GstSeekFlags>(flags),
                                static_cast<GstSeekType>(startType),
                                start,
                                static_cast<GstSeekType>(stopType),
                                stop);
    }
    return false;
}

GstElement* Gst::Element::handler() const
{
    return handler_;
}

void Gst::Element::resetHandler()
{
    handler_ = nullptr;
}
