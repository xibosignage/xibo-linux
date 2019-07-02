#include "MediaParser.hpp"
#include "MediaResources.hpp"
#include "MediaParsersRepo.hpp"

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

ParsedMedia MediaParser::parse(const xml_node& node)
{
    m_node = std::move(node);

    MediaOptions baseOptions{type(), id(), uri(), duration(), geometry()};
    return ParsedMedia{baseOptions, parseAdditonalOptions(m_node), parseAttachedMedia(m_node)};
}

const xml_node& MediaParser::node() const
{
    return m_node;
}

MediaOptions::Type MediaParser::type()
{
    auto type = m_node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type));
    auto render = m_node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Render));

    return {type, render};
}

int MediaParser::id()
{
    return m_node.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Id));
}

Uri MediaParser::uri()
{
    auto uri = m_node.get_optional<std::string>(ResourcesXlf::option(ResourcesXlf::Media::Uri));
    return Validators::validateUri(uri);
}

int MediaParser::duration()
{
    return m_node.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Duration));
}

MediaGeometry MediaParser::geometry()
{
    return MediaGeometry{MediaGeometry::ScaleType::Scaled, MediaGeometry::Align::Left, MediaGeometry::Valign::Top};
}

std::unique_ptr<ParsedMedia> MediaParser::parseAttachedMedia(const xml_node& node)
{
    for(auto [nodeName, attachedNode] : node)
    {
        MediaOptions::Type type{nodeName, ResourcesXlf::Media::NativeRender};
        auto&& parser = MediaParsersRepo::get(type);

        if(parser)
        {
            return std::make_unique<ParsedMedia>(parser->parse(attachedNode));
        }
    }

    return {};
}
