#include "ImageParser.hpp"
#include "utils/utilities.hpp"

ImageParser::ImageParser(const xlf_node& attrs, const xlf_node& options) :
    m_attrs(attrs), m_options(options)
{
}

std::unique_ptr<Media> ImageParser::parse()
{
    int id = m_attrs.template get<int>("id");
    std::string uri = utilities::example_dir() + "/" + m_options.get<std::string>("uri");
    int duration = m_attrs.get<int>("duration");
    int width = m_attrs.get<int>("width");
    int height = m_attrs.get<int>("height");

    auto scale_type = to_scale_type(m_options.get<std::string>("scaleType", "center"));
    auto align = to_align(m_options.get<std::string>("align", "center"));
    auto valign = to_valign(m_options.get<std::string>("valign", "middle"));

    return std::make_unique<Image>(id, Size{width, height}, duration, uri, scale_type, align, valign);
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
