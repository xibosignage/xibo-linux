#include "Decodebin.hpp"
#include "Pad.hpp"

Gst::Decodebin::Decodebin() : Gst::Element("decodebin")
{
    g_signal_connect(element(), "pad-added", reinterpret_cast<GCallback>(onPadAdded), this);
    g_signal_connect(element(), "no-more-pads", reinterpret_cast<GCallback>(noMorePads), this);
}

void Gst::Decodebin::onPadAdded(GstElement* el, GstPad* pad, gpointer data)
{
    reinterpret_cast<Decodebin*>(data)->onPadAddedMem(el, pad, data);
}

void Gst::Decodebin::noMorePads(GstElement* el, gpointer data)
{
    reinterpret_cast<Decodebin*>(data)->noMorePadsMem(el, data);
}

void Gst::Decodebin::onPadAddedMem(GstElement*, GstPad* pad, gpointer)
{
    signalPadAdded_(std::make_shared<Gst::Pad>(pad, false));
}

void Gst::Decodebin::noMorePadsMem(GstElement*, gpointer)
{
    signalNoMorePads_();
}

Gst::RefPtr<Gst::Decodebin> Gst::Decodebin::create()
{
    return Gst::RefPtr<Gst::Decodebin>(new Gst::Decodebin);
}

Gst::SignalPadAdded& Gst::Decodebin::signalPadAdded()
{
    return signalPadAdded_;
}

Gst::SignalsNoMorePads& Gst::Decodebin::signalNoMorePads()
{
    return signalNoMorePads_;
}
