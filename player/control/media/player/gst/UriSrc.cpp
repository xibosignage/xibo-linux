#include "UriSrc.hpp"

#include "common/logger/Logging.hpp"

Gst::UriSrc::UriSrc(std::string_view name, Type type) : Gst::Element(name), type_(type) {}

Gst::RefPtr<Gst::UriSrc> Gst::UriSrc::create(Type type)
{
    switch (type)
    {
        case Type::Local: return Gst::RefPtr<Gst::UriSrc>(new Gst::UriSrc{"filesrc", type});
        case Type::Network: return Gst::RefPtr<Gst::UriSrc>(new Gst::UriSrc{"souphttpsrc", type});
    }
    return nullptr;
}

void Gst::UriSrc::setLocation(const Uri& uri)
{
    if (element())
    {
        std::string path = type_ == Type::Local ? uri.path() : uri.string();
        g_object_set(element(), "location", path.data(), nullptr);
    }
    else
    {
        Log::error("[Gst::UriSrc] Location component is invalid");
    }
}
