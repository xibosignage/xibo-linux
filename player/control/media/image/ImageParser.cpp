#include "ImageParser.hpp"

#include "control/media/creators/MediaResources.hpp"

const MediaGeometry::ScaleType DefaultScaleType = MediaGeometry::ScaleType::Scaled;
const MediaGeometry::Align DefaultAlign = MediaGeometry::Align::Center;
const MediaGeometry::Valign DefaultValign = MediaGeometry::Valign::Middle;

MediaGeometry ImageParser::geometry()
{
    auto scaleType = node().get<MediaGeometry::ScaleType>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::ScaleType), DefaultScaleType);
    auto align = node().get<MediaGeometry::Align>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::Align), DefaultAlign);
    auto valign = node().get<MediaGeometry::Valign>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::Valign), DefaultValign);

    return MediaGeometry{scaleType, align, valign};
}

ExtraOptions ImageParser::parseAdditonalOptions(const xml_node& /*node*/)
{
    return {};
}
