#include "Resources.hpp"

namespace LayoutXlf
{
    const std::string Width = "width";
    const std::string Height = "height";
    const std::string BackgroundPath = "background";
    const std::string BackgroundColor = "bgcolor";
}

namespace RegionXlf
{
    const std::string Width = "width";
    const std::string Height = "height";
    const std::string Left = "left";
    const std::string Top = "top";
    const std::string Zindex = "zindex";
    const std::string Loop = "loop";
}

namespace MediaXlf
{
    const std::string Id = "id";
    const std::string Type = "type";
    const std::string Uri = "uri";
    const std::string Duration = "duration";

    namespace Audio
    {
        const std::string Mute = "mute";
        const std::string Loop = "loop";
        const std::string Volume = "volume";
    }

    namespace Video
    {
        const std::string Mute = "mute";
        const std::string Loop = "loop";
    }

    namespace WebView
    {
        const std::string Transparent = "transparency";
    }

    namespace Image
    {
        const std::string ScaleType = "scaleType";
        const std::string Align = "scaleType";
        const std::string Valign = "scaleType";
    }
}

std::string ResourcesXlf::attr(const std::string& property)
{
    return Attrs + "." + property;
}

std::string ResourcesXlf::option(const std::string& property)
{
    return Options + "." + property;
}

FilePath Resources::directory_;

FilePath Resources::directory()
{
    return directory_;
}

void Resources::setDirectory(const FilePath& directory)
{
    directory_ = directory;
}

ResourcesXlf::LayoutOptions::LayoutOptions(const xlf_node& node)
{
    width = node.get<int>(attr(LayoutXlf::Width));
    height = node.get<int>(attr(LayoutXlf::Height));
    backgroundPath = node.get_optional<std::string>(option(LayoutXlf::BackgroundPath));
    backgroundColor = node.get_optional<std::string>(attr(LayoutXlf::BackgroundColor));
}

ResourcesXlf::RegionOptions::RegionOptions(const xlf_node& node)
{
    width = static_cast<int>(node.get<float>(attr(RegionXlf::Width)));
    height = static_cast<int>(node.get<float>(attr(RegionXlf::Height)));
    left = static_cast<int>(node.get<float>(attr(RegionXlf::Left)));
    top = static_cast<int>(node.get<float>(attr(RegionXlf::Top)));
    zindex = node.get_optional<int>(attr(RegionXlf::Zindex));
    loop = node.get_optional<bool>(option(RegionXlf::Loop));
}

ResourcesXlf::MediaOptions::MediaOptions(const xlf_node& node)
{
    id = node.get<int>(attr(MediaXlf::Id));
    uri = node.get_optional<std::string>(option(MediaXlf::Uri));
    duration = node.get<int>(attr(MediaXlf::Duration));
}

std::string ResourcesXlf::MediaOptions::getType(const xlf_node& node)
{
    return node.get<std::string>(attr(MediaXlf::Type));
}

ResourcesXlf::AudioOptions::AudioOptions(const xlf_node& node) :
    MediaOptions(node)
{
    muted = node.get_optional<bool>(option(MediaXlf::Audio::Mute));
    looped = node.get_optional<bool>(option(MediaXlf::Audio::Loop));
    volume = node.get_optional<int>(option(MediaXlf::Audio::Volume));
}

ResourcesXlf::WebViewOptions::WebViewOptions(const xlf_node& node) :
    MediaOptions(node)
{
    transparent = node.get_optional<bool>(option(MediaXlf::WebView::Transparent));
}

ResourcesXlf::VideoOptions::VideoOptions(const xlf_node& node) :
    MediaOptions(node)
{
    muted = node.get_optional<bool>(option(MediaXlf::Video::Mute));
    looped = node.get_optional<bool>(option(MediaXlf::Video::Loop));
}

ResourcesXlf::ImageOptions::ImageOptions(const xlf_node& node) :
    MediaOptions(node)
{
    scaleType = node.get_optional<std::string>(option(MediaXlf::Image::ScaleType));
    align = node.get_optional<std::string>(option(MediaXlf::Image::Align));
    valign = node.get_optional<std::string>(option(MediaXlf::Image::Valign));
}

