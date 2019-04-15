#pragma once

#include "Element.hpp"
#include <string_view>

namespace Gst
{
    class FileSrc : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::FileSrc> create();
        void setLocation(std::string_view uri);

    private:
        FileSrc();

    };
}
