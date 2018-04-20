#ifndef AUDIOCONVERT_HPP
#define AUDIOCONVERT_HPP

#include "Element.hpp"

namespace Gst
{
    class AudioConvert : public Element
    {
    public:
        static Gst::AudioConvert* create();
    private:
        AudioConvert();

    };
}

#endif // AUDIOCONVERT_HPP
