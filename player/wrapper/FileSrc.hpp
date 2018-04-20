#ifndef FILESRC_HPP
#define FILESRC_HPP

#include "Element.hpp"
#include <string>

namespace Gst
{
    class FileSrc : public Gst::Element
    {
    public:
        static Gst::FileSrc* create();
        void set_location(const std::string& uri);

    private:
        FileSrc();

    };
}

#endif // FILESRC_HPP
