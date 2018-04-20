#ifndef VOLUME_HPP
#define VOLUME_HPP

#include "Element.hpp"

namespace Gst
{
    class Volume : public Element
    {
    public:
        static Gst::Volume* create();
        void set_volume(double volume);

    private:
        Volume();

    };
}

#endif // VOLUME_HPP
