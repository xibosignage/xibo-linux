#pragma once

#include "Element.hpp"
#include <boost/signals2/signal.hpp>

namespace Gst
{
using SignalPadAdded = boost::signals2::signal<void(const Gst::RefPtr<Gst::Pad>&)>;
using SignalsNoMorePads = boost::signals2::signal<void()>;

class Decodebin : public Gst::Element
{
public:
    static Gst::RefPtr<Gst::Decodebin> create();
    SignalPadAdded& signalPadAdded();
    SignalsNoMorePads& signalNoMorePads();

private:
    Decodebin();
    static void onPadAdded(GstElement* el, GstPad* pad, gpointer data);
    static void noMorePads(GstElement* el, gpointer data);

    void onPadAddedMem(GstElement* el, GstPad* pad, gpointer data);
    void noMorePadsMem(GstElement* el, gpointer data);

private:
    SignalPadAdded m_signalPadAdded;
    SignalsNoMorePads m_signalNoMorePads;
};
}
