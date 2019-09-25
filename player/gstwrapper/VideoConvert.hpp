#pragma once

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
