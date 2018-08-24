#pragma once

#include "Element.hpp"

namespace Gst
{
    class Queue : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::Queue> create();
        void set_max_size_buffers(int max_size_buffers);

    private:
        Queue();

    };
}