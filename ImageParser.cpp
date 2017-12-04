#include "ImageParser.hpp"

#include "Image.hpp"

std::shared_ptr<Image> ImageParser::create_from_attrs(const boost::property_tree::ptree& attrs)
{
    auto scaleType = ParserHelper::from_string<Image::ScaleType>(attrs.get<std::string>("scaleType", "center"));
    auto align = ParserHelper::from_string<Image::Align>(attrs.get<std::string>("align", "center"));
    auto valign = ParserHelper::from_string<Image::Valign>(attrs.get<std::string>("valign", "middle"));

    return std::make_shared<Image>(m_id, m_duration, m_render, m_uri, scaleType, align, valign);
}
