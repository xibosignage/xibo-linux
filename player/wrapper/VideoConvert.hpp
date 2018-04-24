#ifndef VIDEOCONVERT_HPP
#define VIDEOCONVERT_HPP

#include "Element.hpp"

namespace Gst
{
    class VideoConvert : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::VideoConvert> create();

    private:
        VideoConvert();

    };
}

#endif // VIDEOCONVERT_HPP
