#include "MediaParser.hpp"
#include "utilities.hpp"

#include "Image.hpp"
#include "Video.hpp"

template <>
std::shared_ptr<Media> MediaParser<Image>::Create(const boost::property_tree::ptree& options)
{
    auto scaleType = utilities::GetValue<ScaleType>(options.get<std::string>("scaleType", "")).value_or(ScaleType::Center);
    auto align = utilities::GetValue<Align>(options.get<std::string>("align", "")).value_or(Align::Center);
    auto valign = utilities::GetValue<Valign>(options.get<std::string>("valign", "")).value_or(Valign::Middle);

    return std::shared_ptr<Media>(new Image(m_id, m_duration, m_render, m_uri, scaleType, align, valign));
}

template <>
std::shared_ptr<Media> MediaParser<Video>::Create(const boost::property_tree::ptree& options)
{
    bool mute = options.get<bool>("mute", false);
    bool loop = options.get<bool>("loop", false);

    return std::shared_ptr<Media>(new Video(m_id, m_duration, m_render, m_uri, mute, loop));
}
