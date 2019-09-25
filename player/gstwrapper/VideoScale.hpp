#pragma once

#include "Element.hpp"

namespace Gst
{
class VideoScale : public Gst::Element
{
public:
    static Gst::RefPtr<Gst::VideoScale> create();

private:
    VideoScale();
};
}
