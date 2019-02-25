#include "ImageBuilder.hpp"

#include "adaptors/GtkImageAdaptor.hpp"

#include <boost/optional/optional.hpp>

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

std::unique_ptr<Image> ImageBuilder::create()
{
    MediaGeometry props{m_scaleType, m_align, m_valign};
    return std::unique_ptr<Image>(new Image{m_id, m_width, m_height, m_uri, props, createHandler()});
}

void ImageBuilder::retrieveMediaOptions(const ImageOptions& opts)
{
    m_scaleType = getScaleTypeOption(opts.scaleType());
    m_align = getAlignOption(opts.align());
    m_valign = getValignOption(opts.valign());
}

std::unique_ptr<IImageAdaptor> ImageBuilder::createDefaultHandler()
{
    return std::make_unique<GtkImageAdaptor>();
}

MediaGeometry::ScaleType ImageBuilder::getScaleTypeOption(const boost::optional<std::string>& scaleTypeOpt)
{
    auto scaleType = scaleTypeOpt.value_or(DEFAULT_SCALE_TYPE);

    if(scaleType == "center")
        return MediaGeometry::ScaleType::Scaled;
    else if(scaleType == "stretch")
        return MediaGeometry::ScaleType::Stretch;

    throw std::invalid_argument("ScaleType is not valid");
}

MediaGeometry::Align ImageBuilder::getAlignOption(const boost::optional<std::string>& alignOpt)
{
    auto align = alignOpt.value_or(DEFAULT_ALIGN);

    if(align == "left")
        return MediaGeometry::Align::Left;
    else if(align == "center")
        return MediaGeometry::Align::Center;
    else if(align == "right")
        return MediaGeometry::Align::Right;

    throw std::invalid_argument("Align is not valid");
}

MediaGeometry::Valign ImageBuilder::getValignOption(const boost::optional<std::string>& valignOpt)
{
    auto valign = valignOpt.value_or(DEFAULT_VALIGN);

    if(valign == "top")
        return MediaGeometry::Valign::Top;
    else if(valign == "middle")
        return MediaGeometry::Valign::Middle;
    else if(valign == "bottom")
        return MediaGeometry::Valign::Bottom;

    throw std::invalid_argument("Valign is not valid");
}
