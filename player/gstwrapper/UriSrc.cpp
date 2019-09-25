#include "UriSrc.hpp"

#include "common/logger/Logging.hpp"

Gst::UriSrc::UriSrc(std::string_view name, Uri::Scheme scheme) : Gst::Element(name), m_fileType(scheme) {}

Gst::RefPtr<Gst::UriSrc> Gst::UriSrc::create(Uri::Scheme scheme)
{
    switch (scheme)
    {
        case Uri::Scheme::FILE: return Gst::RefPtr<Gst::UriSrc>(new Gst::UriSrc{"filesrc", scheme});
        case Uri::Scheme::HTTP:
        case Uri::Scheme::HTTPS: return Gst::RefPtr<Gst::UriSrc>(new Gst::UriSrc{"souphttpsrc", scheme});
        default: return nullptr;
    }
}

void Gst::UriSrc::setLocation(const Uri& uri)
{
    if (element())
    {
        std::string path = m_fileType == Uri::Scheme::FILE ? uri.path() : uri.string();
        g_object_set(element(), "location", path.data(), nullptr);
    }
    else
    {
        Log::error("[Gst::UriSrc] Location component is invalid");
    }
}
