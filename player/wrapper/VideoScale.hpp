#ifndef VIDEOSCALE_HPP
#define VIDEOSCALE_HPP

#include "Element.hpp"

namespace Gst
{
    class VideoScale : public Element
    {
    public:
        static Gst::VideoScale* create();
    private:
        VideoScale();

    };
}

#endif // VIDEOSCALE_HPP
