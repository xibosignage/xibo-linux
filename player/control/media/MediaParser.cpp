#include "MediaParser.hpp"

#include "MediaResources.hpp"
#include "control/common/Validators.hpp"

std::istream& operator >>(std::istream& in, MediaGeometry::ScaleType& scaleType)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Geometry::Scaled || temp == ResourcesXlf::Media::Geometry::Aspect)
        scaleType = MediaGeometry::ScaleType::Scaled;
    else if(temp == ResourcesXlf::Media::Geometry::Stretch)
        scaleType = MediaGeometry::ScaleType::Stretch;

    return in;
}

std::istream& operator >>(std::istream& in, MediaGeometry::Align& align)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Geometry::LeftAlign)
        align = MediaGeometry::Align::Left;
    else if(temp == ResourcesXlf::Media::Geometry::CenterAlign)
        align = MediaGeometry::Align::Center;
    else if(temp == ResourcesXlf::Media::Geometry::RightAlign)
        align = MediaGeometry::Align::Right;

    return in;
}

std::istream& operator >>(std::istream& in, MediaGeometry::Valign& valign)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Geometry::TopValign)
        valign = MediaGeometry::Valign::Top;
    else if(temp == ResourcesXlf::Media::Geometry::MiddleValign)
        valign = MediaGeometry::Valign::Middle;
    else if(temp == ResourcesXlf::Media::Geometry::BottomValign)
        valign = MediaGeometry::Valign::Bottom;

    return in;
}

MediaParser::MediaParser(const xml_node& node) :
    m_node(node)
{
}

const xml_node& MediaParser::node() const
{
    return m_node;
}

MediaOptions MediaParser::baseOptions()
{
    m_id = m_node.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Id));
    m_uri = m_node.get_optional<std::string>(ResourcesXlf::option(ResourcesXlf::Media::Uri));
    m_duration = m_node.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Duration));

    return MediaOptions{id(), uri(), duration(), geometry()};
}

int MediaParser::id()
{
    return m_id;
}

Uri MediaParser::uri()
{
    return Validators::validateUri(m_uri);
}

int MediaParser::duration()
{
    return m_duration;
}

MediaGeometry MediaParser::geometry()
{
    return MediaGeometry{MediaGeometry::ScaleType::Scaled, MediaGeometry::Align::Left, MediaGeometry::Valign::Top};
}
