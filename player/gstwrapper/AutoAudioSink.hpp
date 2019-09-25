#pragma once

#include "Element.hpp"

namespace Gst
{
class AutoAudioSink : public Gst::Element
{
public:
    static Gst::RefPtr<Gst::AutoAudioSink> create();

private:
    AutoAudioSink();
};
}
