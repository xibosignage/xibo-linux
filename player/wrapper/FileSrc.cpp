#include "FileSrc.hpp"

Gst::FileSrc::FileSrc()
{
    m_element = gst_element_factory_make("filesrc", nullptr);
}

Gst::FileSrc* Gst::FileSrc::create()
{
    return new FileSrc;
}

void Gst::FileSrc::set_location(const std::string& uri)
{
    g_object_set(m_element, "location", uri.c_str(), nullptr);
}
