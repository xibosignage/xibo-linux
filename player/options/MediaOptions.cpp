#include "MediaOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

MediaOptions::MediaOptions(int id, boost::optional<std::string> path, int duration) :
    m_id(id), m_path(path), m_duration(duration)
{
}

MediaOptions::MediaOptions(const xml_node& node)
{
    m_id = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Id));
    m_path = node.get_optional<std::string>(ResourcesXlf::option(ResourcesXlf::Media::Path));
    m_duration = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Duration));
}

std::string MediaOptions::getType(const xml_node& node)
{
    return node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type));
}

int MediaOptions::id() const
{
    return m_id;
}

boost::optional<std::string> MediaOptions::path() const
{
    return m_path;
}

int MediaOptions::duration() const
{
    return m_duration;
}

void MediaOptions::setId(int id)
{
    m_id = id;
}

void MediaOptions::setUri(const boost::optional<std::string>& uri)
{
    m_path = uri;
}

void MediaOptions::setDuration(int duration)
{
    m_duration = duration;
}
