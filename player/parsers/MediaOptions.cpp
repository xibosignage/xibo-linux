#include "MediaOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::MediaOptions::MediaOptions(const xml_node& node)
{
    m_id = node.get<int>(attr(Media::Id));
    m_path = node.get_optional<std::string>(option(Media::Uri));
    m_duration = node.get<int>(attr(Media::Duration));
}

std::string ResourcesXlf::MediaOptions::getType(const xml_node& node)
{
    return node.get<std::string>(attr(Media::Type));
}

int ResourcesXlf::MediaOptions::id() const
{
    return m_id;
}

boost::optional<std::string> ResourcesXlf::MediaOptions::path() const
{
    return m_path;
}

int ResourcesXlf::MediaOptions::duration() const
{
    return m_duration;
}

void ResourcesXlf::MediaOptions::setId(int id)
{
    m_id = id;
}

void ResourcesXlf::MediaOptions::setUri(const boost::optional<std::string>& uri)
{
    m_path = uri;
}

void ResourcesXlf::MediaOptions::setDuration(int duration)
{
    m_duration = duration;
}
