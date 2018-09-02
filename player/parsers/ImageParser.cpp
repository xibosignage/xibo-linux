#include "ImageParser.hpp"
#include "utils/utilities.hpp"

ImageParser::ImageParser(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaParser(parentNode, mediaNode)
{
}

std::unique_ptr<Media> ImageParser::doParse()
{
    int id = attrs().template get<int>("id");
    auto uri = utils::resourcesDir() / options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");
    int width = parentNode().get_child("<xmlattr>").get<double>("width");
    int height = parentNode().get_child("<xmlattr>").get<double>("height");

    auto scaleType = toScaleType(options().get<std::string>("scaleType", "center"));
    auto align = toAlign(options().get<std::string>("align", "center"));
    auto valign = toValign(options().get<std::string>("valign", "middle"));

    return std::make_unique<Image>(id, width, height, duration, uri.string(), scaleType, align, valign);
}

Image::ScaleType ImageParser::toScaleType(const std::string& scale_type)
{
    if(scale_type == "center")
        return Image::ScaleType::Scaled;
    else if(scale_type == "stretch")
        return Image::ScaleType::Stretch;
    else
        return Image::ScaleType::Invalid;
}

Image::Align ImageParser::toAlign(const std::string& align)
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

Image::Valign ImageParser::toValign(const std::string& valign)
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
