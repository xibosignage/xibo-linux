#pragma once

#include "Element.hpp"

namespace Gst
{
    class Capsfilter : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::Capsfilter> create();
        void setCaps(Gst::RefPtr<Gst::Caps> caps);

    private:
        Capsfilter();

    private:
        Gst::RefPtr<Gst::Caps> m_caps;
    };
}
