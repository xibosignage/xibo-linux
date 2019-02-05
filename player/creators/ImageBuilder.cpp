#include "ImageBuilder.hpp"
#include "adaptors/GtkImageAdaptor.hpp"

#include <boost/optional/optional.hpp>

std::unique_ptr<Image> ImageBuilder::build()
{
    MediaGeometry props{m_scaleType, m_align, m_valign};
    auto image = std::make_unique<Image>(m_id, m_width, m_height, m_path, props, createAdaptor());
    prepareCommonParams(*image);
    return image;
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

MediaGeometry::ScaleType ImageBuilder::toScaleType(std::string_view scaleType)
{
    if(scaleType == "center")
        return MediaGeometry::ScaleType::Scaled;
    else if(scaleType == "stretch")
        return MediaGeometry::ScaleType::Stretch;

    throw std::invalid_argument("ScaleType is not valid");
}

MediaGeometry::Align ImageBuilder::toAlign(std::string_view align)
{
    if(align == "left")
        return MediaGeometry::Align::Left;
    else if(align == "center")
        return MediaGeometry::Align::Center;
    else if(align == "right")
        return MediaGeometry::Align::Right;

    throw std::invalid_argument("Align is not valid");
}

MediaGeometry::Valign ImageBuilder::toValign(std::string_view valign)
{
    if(valign == "top")
        return MediaGeometry::Valign::Top;
    else if(valign == "middle")
        return MediaGeometry::Valign::Middle;
    else if(valign == "bottom")
        return MediaGeometry::Valign::Bottom;

    throw std::invalid_argument("Valign is not valid");
}
