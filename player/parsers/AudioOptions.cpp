#include "AudioOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

const int DEFAULT_DURATION = 0;

ResourcesXlf::AudioOptions::AudioOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_muted = node.get_optional<bool>(option(Media::Audio::Mute));
    m_looped = node.get_optional<bool>(option(Media::Audio::Loop));
    m_volume = node.get_optional<int>(option(Media::Audio::Volume));
}

ResourcesXlf::AudioOptions::AudioOptions(const xml_node& node, AudioNodeTag)
{
    auto uriNode = node.get_child(Media::Path);

    setId(uriNode.get<int>(attr("mediaId")));
    setUri(uriNode.get_value<std::string>());
    setDuration(DEFAULT_DURATION);

    m_muted = uriNode.get_optional<bool>(attr(Media::Audio::Mute));
    m_looped = uriNode.get_optional<bool>(attr(Media::Audio::Loop));
    m_volume = uriNode.get_optional<int>(attr(Media::Audio::Volume));
}

boost::optional<bool> ResourcesXlf::AudioOptions::muted() const
{
    return m_muted;
}

boost::optional<bool> ResourcesXlf::AudioOptions::looped() const
{
    return m_looped;
}

boost::optional<int> ResourcesXlf::AudioOptions::volume() const
{
    return m_volume;
}
