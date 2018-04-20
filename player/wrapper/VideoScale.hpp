#ifndef VIDEOSCALE_HPP
#define VIDEOSCALE_HPP

#include "Element.hpp"

namespace Gst
{
    class VideoScale : public Gst::Element
    {
    public:
        static Gst::VideoScale* create();
    private:
        VideoScale();

    };
}

#endif // VIDEOSCALE_HPP
