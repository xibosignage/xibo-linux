#include "VideoOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

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
