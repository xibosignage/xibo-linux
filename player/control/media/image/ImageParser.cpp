#include "ImageParser.hpp"

#include "control/media/Media.hpp"
#include "control/media/MediaResources.hpp"
#include "control/media/image/ImageFactory.hpp"

const MediaGeometry::ScaleType DefaultScaleType = MediaGeometry::ScaleType::Scaled;
const MediaGeometry::Align DefaultAlign = MediaGeometry::Align::Center;
const MediaGeometry::Valign DefaultValign = MediaGeometry::Valign::Middle;

MediaGeometry ImageParser::geometryFrom(const XmlNode& node)
{
    auto scaleType = node.get<MediaGeometry::ScaleType>(XlfResources::Media::Geometry::ScaleType, DefaultScaleType);
    auto align = node.get<MediaGeometry::Align>(XlfResources::Media::Geometry::Align, DefaultAlign);
    auto valign = node.get<MediaGeometry::Valign>(XlfResources::Media::Geometry::Valign, DefaultValign);

    return MediaGeometry{scaleType, align, valign};
}

std::unique_ptr<Xibo::Media> ImageParser::createMedia(const MediaOptions& options,
                                                      const XmlNode& /*node*/,
                                                      int width,
                                                      int height)
{
    ImageFactory factory;
    return factory.create(options, width, height);
}
