#include "MediaParser.hpp"

#include "MediaResources.hpp"
#include "control/common/Validators.hpp"

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

    return MediaOptions{id(), uri(), duration()};
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
