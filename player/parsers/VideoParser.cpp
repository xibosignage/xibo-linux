#include "VideoParser.hpp"
#include "media/Video.hpp"

VideoParser::VideoParser(const xlf_node& attrs, const xlf_node& options) :
    m_attrs(attrs), m_options(options)
{
}

std::unique_ptr<Media> VideoParser::parse()
{
    int id = m_attrs.template get<int>("id");
    std::string uri = m_options.get<std::string>("uri");
    int duration = m_attrs.get<int>("duration");
    int width = m_attrs.get<int>("width");
    int height = m_attrs.get<int>("height");

    bool mute = m_options.get<bool>("mute", false);
    bool loop = m_options.get<bool>("loop", false);

    return std::make_unique<Video>(id, Size{width, height}, duration, uri, mute, loop);
}
