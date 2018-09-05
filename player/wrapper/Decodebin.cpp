#include "Decodebin.hpp"
#include "Pad.hpp"

#include "utils/BindWrapper.hpp"

namespace ph = std::placeholders;

Gst::Decodebin::Decodebin()
{
    setElement(gst_element_factory_make("decodebin", nullptr));

    auto onPadAdded = get_wrapper<0, void, GstElement*, GstPad*, gpointer>(std::bind(&Decodebin::onPadAdded, this, ph::_1, ph::_2, ph::_3));
    g_signal_connect_data(element(), "pad-added", reinterpret_cast<GCallback>(onPadAdded), nullptr, nullptr, static_cast<GConnectFlags>(0));

    auto noMorePads = get_wrapper<1, void, GstElement*, gpointer>(std::bind(&Decodebin::noMorePads, this, ph::_1, ph::_2));
    g_signal_connect_data(element(), "no-more-pads", reinterpret_cast<GCallback>(noMorePads), nullptr, nullptr, static_cast<GConnectFlags>(0));
}

void Gst::Decodebin::onPadAdded(GstElement*, GstPad* pad, gpointer)
{
    m_signalPadAdded.emit(std::make_shared<Gst::Pad>(pad, false));
}

void Gst::Decodebin::noMorePads(GstElement*, gpointer)
{
    m_signalNoMorePads.emit();
}

Gst::RefPtr<Gst::Decodebin> Gst::Decodebin::create()
{
    return std::shared_ptr<Gst::Decodebin>(new Gst::Decodebin);
}

sigc::signal<void(const Gst::RefPtr<Gst::Pad>&)>& Gst::Decodebin::signalPadAdded()
{
    return m_signalPadAdded;
}

sigc::signal<void()>& Gst::Decodebin::signalNoMorePads()
{
    return m_signalNoMorePads;
}
