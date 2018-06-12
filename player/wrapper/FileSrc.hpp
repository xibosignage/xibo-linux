#pragma once

#include "Element.hpp"
#include <string>

namespace Gst
{
    class FileSrc : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::FileSrc> create();
        void set_location(const std::string& uri);

    private:
        FileSrc();

    };
}
