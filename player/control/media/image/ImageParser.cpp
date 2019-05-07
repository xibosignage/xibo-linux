#include "ImageParser.hpp"

#include "control/media/MediaResources.hpp"

const MediaGeometry::ScaleType DEFAULT_SCALE_TYPE = MediaGeometry::ScaleType::Scaled;
const MediaGeometry::Align DEFAULT_ALIGN = MediaGeometry::Align::Center;
const MediaGeometry::Valign DEFAULT_VALIGN = MediaGeometry::Valign::Middle;

ImageParser::ImageParser(const xml_node& node) :
    MediaParser(node)
{

}

ImageOptions ImageParser::parse()
{
    auto options = baseOptions();

    return ImageOptions{options};
}

MediaGeometry ImageParser::geometry()
{
    auto scaleType = node().get<MediaGeometry::ScaleType>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::ScaleType), DEFAULT_SCALE_TYPE);
    auto align = node().get<MediaGeometry::Align>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::Align), DEFAULT_ALIGN);
    auto valign = node().get<MediaGeometry::Valign>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::Valign), DEFAULT_VALIGN);

    return MediaGeometry{scaleType, align, valign};
}

