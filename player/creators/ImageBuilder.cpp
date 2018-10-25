#include "ImageBuilder.hpp"

#include "adaptors/GtkImageAdaptor.hpp"

std::unique_ptr<IMedia> ImageBuilder::doBuild()
{
    ImageProperties props{m_scaleType, m_align, m_valign};
    return std::make_unique<Image>(m_width, m_height, m_path, props, std::make_unique<GtkImageAdaptor>());
}

ImageBuilder& ImageBuilder::width(int width)
{
    m_width = width;
    return *this;
}

ImageBuilder& ImageBuilder::height(int height)
{
    m_height = height;
    return *this;
}

ImageBuilder& ImageBuilder::scaleType(const boost::optional<std::string>& scaleType)
{
    m_scaleType = toScaleType(scaleType.value_or("center"));
    return *this;
}

ImageBuilder& ImageBuilder::align(const boost::optional<std::string>& align)
{
    m_align = toAlign(align.value_or("center"));
    return *this;
}

ImageBuilder& ImageBuilder::valign(const boost::optional<std::string>& valign)
{
    m_valign = toValign(valign.value_or("middle"));
    return *this;
}

ImageProperties::ScaleType ImageBuilder::toScaleType(const std::string& scale_type)
{
    if(scale_type == "center")
        return ImageProperties::ScaleType::Scaled;
    else if(scale_type == "stretch")
        return ImageProperties::ScaleType::Stretch;
    else
        return ImageProperties::ScaleType::Invalid;
}

ImageProperties::Align ImageBuilder::toAlign(const std::string& align)
{
    if(align == "left")
        return ImageProperties::Align::Left;
    else if(align == "center")
        return ImageProperties::Align::Center;
    else if(align == "right")
        return ImageProperties::Align::Right;
    else
        return ImageProperties::Align::Invalid;
}

ImageProperties::Valign ImageBuilder::toValign(const std::string& valign)
{
    if(valign == "top")
        return ImageProperties::Valign::Top;
    else if(valign == "middle")
        return ImageProperties::Valign::Middle;
    else if(valign == "bottom")
        return ImageProperties::Valign::Bottom;
    else
        return ImageProperties::Valign::Invalid;
}
