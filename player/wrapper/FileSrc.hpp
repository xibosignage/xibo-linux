#pragma once

#include "Element.hpp"
#include <string>

namespace Gst
{
    class FileSrc : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::FileSrc> create();
        void setLocation(const std::string& uri);

    private:
        FileSrc();

    };
}
