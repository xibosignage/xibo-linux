#include "VideoOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::VideoOptions::VideoOptions(int id,
                                         boost::optional<std::string> path,
                                         int duration,
                                         boost::optional<bool> muted,
                                         boost::optional<bool> looped) :
    MediaOptions(id, path, duration), m_muted(muted), m_looped(looped)
{
}

ResourcesXlf::VideoOptions::VideoOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_muted = node.get_optional<bool>(option(Media::Video::Mute));
    m_looped = node.get_optional<bool>(option(Media::Video::Loop));
}

boost::optional<bool> ResourcesXlf::VideoOptions::muted() const
{
    return m_muted;
}

boost::optional<bool> ResourcesXlf::VideoOptions::looped() const
{
    return m_looped;
}
