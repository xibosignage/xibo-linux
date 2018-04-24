#ifndef VOLUME_HPP
#define VOLUME_HPP

#include "Element.hpp"

namespace Gst
{
    class Volume : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::Volume> create();
        void set_volume(double volume);

    private:
        Volume();

    };
}

#endif // VOLUME_HPP
