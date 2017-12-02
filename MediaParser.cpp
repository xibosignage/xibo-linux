#include "MediaParser.hpp"

template <>
std::shared_ptr<Media> MediaParser<Image>::create(const boost::property_tree::ptree& options)
{
    auto scaleType = ParserHelper::from_string<Image::ScaleType>(options.get<std::string>("scaleType", "center"));
    auto align = ParserHelper::from_string<Image::Align>(options.get<std::string>("align", "center"));
    auto valign = ParserHelper::from_string<Image::Valign>(options.get<std::string>("valign", "middle"));

    return std::shared_ptr<Media>(new Image(m_id, m_duration, m_render, m_uri, scaleType, align, valign));
}

template<>
std::shared_ptr<Media> MediaParser<Video>::create(const boost::property_tree::ptree& options)
{
    bool mute = options.get<bool>("mute", false);
    bool loop = options.get<bool>("loop", false);

    return std::shared_ptr<Media>(new Video(m_id, m_duration, m_render, m_uri, mute, loop));
}
