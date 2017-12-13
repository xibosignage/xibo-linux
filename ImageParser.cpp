#include "ImageParser.hpp"

ParsedImage ImageParser::parse()
{
    auto scale_type = from_scale_type(m_options.get<std::string>("scaleType", "center"));
    auto align = from_align(m_options.get<std::string>("align", "center"));
    auto valign = from_valign(m_options.get<std::string>("valign", "middle"));

    return ParsedImage{m_id, m_duration, m_uri, scale_type, align, valign};
}

Image::ScaleType ImageParser::from_scale_type(const std::__cxx11::string& option_name)
{
    if(option_name == "center")
        return Image::ScaleType::Center;
    else if(option_name == "stretch")
        return Image::ScaleType::Stretch;
    else
        return Image::ScaleType::Invalid;
}

Image::Align ImageParser::from_align(const std::__cxx11::string& option_name)
{
    if(option_name == "left")
        return Image::Align::Left;
    else if(option_name == "center")
        return Image::Align::Center;
    else if(option_name == "right")
        return Image::Align::Right;
    else
        return Image::Align::Invalid;
}

Image::Valign ImageParser::from_valign(const std::__cxx11::string& option_name)
{
    if(option_name == "top")
        return Image::Valign::Top;
    else if(option_name == "middle")
        return Image::Valign::Middle;
    else if(option_name == "bottom")
        return Image::Valign::Bottom;
    else
        return Image::Valign::Invalid;
}
