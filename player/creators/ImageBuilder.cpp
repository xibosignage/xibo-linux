#include "ImageBuilder.hpp"

#include "media/Image.hpp"
#include "adaptors/GtkImageAdaptor.hpp"

const std::string DEFAULT_SCALE_TYPE = "center";
const std::string DEFAULT_ALIGN = "center";
const std::string DEFAULT_VALIGN = "middle";

std::unique_ptr<IMedia> ImageBuilder::doBuild()
{
    return createImage();
}

std::unique_ptr<Image> ImageBuilder::createImage()
{
    ImageProperties props{m_scaleType, m_align, m_valign};
    return std::make_unique<Image>(m_width, m_height, m_path, props, createAdaptor());
}

std::unique_ptr<IImageAdaptor> ImageBuilder::createAdaptor()
{
    return std::make_unique<GtkImageAdaptor>();
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
    m_scaleType = toScaleType(scaleType.value_or(DEFAULT_SCALE_TYPE));
    return *this;
}

ImageBuilder& ImageBuilder::align(const boost::optional<std::string>& align)
{
    m_align = toAlign(align.value_or(DEFAULT_ALIGN));
    return *this;
}

ImageBuilder& ImageBuilder::valign(const boost::optional<std::string>& valign)
{
    m_valign = toValign(valign.value_or(DEFAULT_VALIGN));
    return *this;
}

ImageProperties::ScaleType ImageBuilder::toScaleType(const std::string& scaleType)
{
    if(scaleType == "center")
        return ImageProperties::ScaleType::Scaled;
    else if(scaleType == "stretch")
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
