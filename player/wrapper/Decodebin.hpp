#ifndef DECODEBIN_HPP
#define DECODEBIN_HPP

#include "Element.hpp"
#include <sigc++/signal.h>

namespace Gst
{
    class Decodebin : public Gst::Element
    {
    public:
        static Gst::Decodebin* create();
        sigc::signal<void(Gst::Pad*)>& signal_pad_added();
        sigc::signal<void()>& signal_no_more_pads();

    private:
        Decodebin();
        void on_pad_added(GstElement* el, GstPad* pad, gpointer data);
        void no_more_pads(GstElement* el, gpointer data);

    private:
        sigc::signal<void(Gst::Pad*)> m_signal_pad_added;
        sigc::signal<void()> m_signal_no_more_pads;

    };
}
#endif // DECODEBIN_HPP
