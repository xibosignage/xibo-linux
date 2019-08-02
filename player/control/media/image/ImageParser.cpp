#include "ImageParser.hpp"

#include "control/media/MediaResources.hpp"

const MediaGeometry::ScaleType DefaultScaleType = MediaGeometry::ScaleType::Scaled;
const MediaGeometry::Align DefaultAlign = MediaGeometry::Align::Center;
const MediaGeometry::Valign DefaultValign = MediaGeometry::Valign::Middle;

MediaGeometry ImageParser::geometryFrom(const ptree_node& node)
{
    auto scaleType = node.get<MediaGeometry::ScaleType>(XlfResources::Media::Geometry::ScaleType, DefaultScaleType);
    auto align = node.get<MediaGeometry::Align>(XlfResources::Media::Geometry::Align, DefaultAlign);
    auto valign = node.get<MediaGeometry::Valign>(XlfResources::Media::Geometry::Valign, DefaultValign);

    return MediaGeometry{scaleType, align, valign};
}

ExtraOptions ImageParser::parseExtraOptionsImpl(const ptree_node& /*node*/)
{
    return {};
}
