#include "FileSrc.hpp"

Gst::FileSrc::FileSrc() :
    Gst::Element("filesrc")
{
}

Gst::RefPtr<Gst::FileSrc> Gst::FileSrc::create()
{
    return std::shared_ptr<Gst::FileSrc>(new Gst::FileSrc);
}

void Gst::FileSrc::setLocation(std::string_view uri)
{
    g_object_set(element(), "location", uri.data(), nullptr);
}
