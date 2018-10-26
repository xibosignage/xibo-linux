#include "MediaOptions.hpp"
#include "XlfResources.hpp"

ResourcesXlf::MediaOptions::MediaOptions(const xlf_node& node)
{
    m_id = node.get<int>(attr(Media::Id));
    m_uri = node.get_optional<std::string>(option(Media::Uri));
    m_duration = node.get<int>(attr(Media::Duration));
}

std::string ResourcesXlf::MediaOptions::getType(const xlf_node& node)
{
    return node.get<std::string>(attr(Media::Type));
}

int ResourcesXlf::MediaOptions::id() const
{
    return m_id;
}

boost::optional<std::string> ResourcesXlf::MediaOptions::uri() const
{
    return m_uri;
}

int ResourcesXlf::MediaOptions::duration() const
{
    return m_duration;
}
