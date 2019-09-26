#pragma once

#include "Element.hpp"

namespace Gst
{
    class Queue : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::Queue> create();
        void setMaxSizeBuffers(int maxSizeBuffers);

    private:
        Queue();
    };
}
