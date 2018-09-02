#pragma once

#include "Element.hpp"
#include <sigc++/signal.h>

namespace Gst
{
    class Decodebin : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::Decodebin> create();
        sigc::signal<void(const Gst::RefPtr<Gst::Pad>&)>& signalPadAdded();
        sigc::signal<void()>& signalNoMorePads();

    private:
        Decodebin();
        void onPadAdded(GstElement* el, GstPad* pad, gpointer data);
        void noMorePads(GstElement* el, gpointer data);

    private:
        sigc::signal<void(const Gst::RefPtr<Gst::Pad>&)> m_signalPadAdded;
        sigc::signal<void()> m_signalNoMorePads;

    };
}
