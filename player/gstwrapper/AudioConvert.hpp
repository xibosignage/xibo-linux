#pragma once

#include "Element.hpp"

namespace Gst
{
class AudioConvert : public Gst::Element
{
public:
    static Gst::RefPtr<Gst::AudioConvert> create();

private:
    AudioConvert();
};
}
