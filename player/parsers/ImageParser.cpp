#include "ImageParser.hpp"
#include "utils/utilities.hpp"

ImageParser::ImageParser(const xlf_node& parent_node, const xlf_node& media_node) :
    MediaParser(parent_node, media_node)
{
}

std::unique_ptr<Media> ImageParser::parse()
{
    int id = attrs().template get<int>("id");
    std::string uri = utils::example_dir() + "/" + options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");
    int width = parent_node().get_child("<xmlattr>").get<double>("width");
    int height = parent_node().get_child("<xmlattr>").get<double>("height");

    auto scale_type = to_scale_type(options().get<std::string>("scaleType", "center"));
    auto align = to_align(options().get<std::string>("align", "center"));
    auto valign = to_valign(options().get<std::string>("valign", "middle"));

    auto image = std::make_unique<Image>(id, Size{width, height}, duration, uri, scale_type, align, valign);
    image->attach_audio(parse_audio_node());
    return image;
}

Image::ScaleType ImageParser::to_scale_type(const std::string& scale_type)
{
    if(scale_type == "center")
        return Image::ScaleType::Scaled;
    else if(scale_type == "stretch")
        return Image::ScaleType::Stretch;
    else
        return Image::ScaleType::Invalid;
}

Image::Align ImageParser::to_align(const std::string& align)
{
    if(align == "left")
        return Image::Align::Left;
    else if(align == "center")
        return Image::Align::Center;
    else if(align == "right")
        return Image::Align::Right;
    else
        return Image::Align::Invalid;
}

Image::Valign ImageParser::to_valign(const std::string& valign)
{
    if(valign == "top")
        return Image::Valign::Top;
    else if(valign == "middle")
        return Image::Valign::Middle;
    else if(valign == "bottom")
        return Image::Valign::Bottom;
    else
        return Image::Valign::Invalid;
}
