#include "Decodebin.hpp"
#include "utils/BindWrapper.hpp"

namespace ph = std::placeholders;

Gst::Decodebin::Decodebin()
{
    m_element = gst_element_factory_make("decodebin", nullptr);

    auto on_pad_added = get_wrapper<0, void, GstElement*, GstPad*, gpointer>(std::bind(&Decodebin::on_pad_added, this, ph::_1, ph::_2, ph::_3));
    g_signal_connect_data(m_element, "pad-added", reinterpret_cast<GCallback>(on_pad_added), nullptr, nullptr, static_cast<GConnectFlags>(0));

    auto no_more_pads = get_wrapper<1, void, GstElement*, gpointer>(std::bind(&Decodebin::no_more_pads, this, ph::_1, ph::_2));
    g_signal_connect_data(m_element, "no-more-pads", reinterpret_cast<GCallback>(no_more_pads), nullptr, nullptr, static_cast<GConnectFlags>(0));
}

void Gst::Decodebin::on_pad_added(GstElement*, GstPad* pad, gpointer)
{
    m_signal_pad_added.emit(new Gst::Pad(pad));
}

void Gst::Decodebin::no_more_pads(GstElement*, gpointer)
{
    m_signal_no_more_pads.emit();
}

Gst::Decodebin* Gst::Decodebin::create()
{
    return new Decodebin;
}

sigc::signal<void(Gst::Pad*)>& Gst::Decodebin::signal_pad_added()
{
    return m_signal_pad_added;
}

sigc::signal<void()>& Gst::Decodebin::signal_no_more_pads()
{
    return m_signal_no_more_pads;
}
