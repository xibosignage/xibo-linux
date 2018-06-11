#pragma once

#include "Element.hpp"

namespace Gst
{
    class Capsfilter : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::Capsfilter> create();
        void set_caps(Gst::RefPtr<Gst::Caps> caps);

    private:
        Capsfilter();

    private:
        Gst::RefPtr<Gst::Caps> m_caps;

    };
}
