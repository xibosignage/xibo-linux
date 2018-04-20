#ifndef AUTOAUDIOSINK_HPP
#define AUTOAUDIOSINK_HPP

#include "Element.hpp"

namespace Gst
{
    class AutoAudioSink : public Element
    {
    public:
        static Gst::AutoAudioSink* create();
    private:
        AutoAudioSink();

    };
}

#endif // AUTOAUDIOSINK_HPP
