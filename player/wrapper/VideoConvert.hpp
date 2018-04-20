#ifndef VIDEOCONVERT_HPP
#define VIDEOCONVERT_HPP

#include "Element.hpp"

namespace Gst
{
    class VideoConvert : public Element
    {
    public:
        static Gst::VideoConvert* create();
    private:
        VideoConvert();

    };
}

#endif // VIDEOCONVERT_HPP
