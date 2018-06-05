#pragma once

#include "Element.hpp"

namespace Gst
{
    class Volume : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::Volume> create();
        void set_volume(double volume);
        double get_volume() const;

    private:
        Volume();

    };
}
