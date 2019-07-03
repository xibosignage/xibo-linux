#include "ImageParser.hpp"

#include "control/media/creators/MediaResources.hpp"

const MediaGeometry::ScaleType DEFAULT_SCALE_TYPE = MediaGeometry::ScaleType::Scaled;
const MediaGeometry::Align DEFAULT_ALIGN = MediaGeometry::Align::Center;
const MediaGeometry::Valign DEFAULT_VALIGN = MediaGeometry::Valign::Middle;

#include "common/logger/Logging.hpp"

MediaGeometry ImageParser::geometry()
{
    auto scaleType = node().get<MediaGeometry::ScaleType>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::ScaleType), DEFAULT_SCALE_TYPE);
    auto align = node().get<MediaGeometry::Align>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::Align), DEFAULT_ALIGN);
    auto valign = node().get<MediaGeometry::Valign>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::Valign), DEFAULT_VALIGN);

    return MediaGeometry{scaleType, align, valign};
}

ExtraOptions ImageParser::parseAdditonalOptions(const xml_node& /*node*/)
{
    return {};
}
