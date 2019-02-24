#include "AudioOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

const int DEFAULT_DURATION = 0;

AudioOptions::AudioOptions(int id,
                           boost::optional<std::string> path,
                           int duration,
                           boost::optional<AudioOptions::Mute> muted,
                           boost::optional<AudioOptions::Loop> looped,
                           boost::optional<int> volume) :
    MediaOptions(id, path, duration), m_muted(muted), m_looped(looped), m_volume(volume)
{
}

AudioOptions::AudioOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_muted = node.get_optional<Mute>(ResourcesXlf::option(ResourcesXlf::Media::Audio::Mute));
    m_looped = node.get_optional<Loop>(ResourcesXlf::option(ResourcesXlf::Media::Audio::Loop));
    m_volume = node.get_optional<int>(ResourcesXlf::option(ResourcesXlf::Media::Audio::Volume));
}

AudioOptions::AudioOptions(const xml_node& node, AudioNodeTag)
{
    auto uriNode = node.get_child(ResourcesXlf::Media::Path);

    setId(uriNode.get<int>(ResourcesXlf::attr("mediaId")));
    setUri(uriNode.get_value<std::string>());
    setDuration(DEFAULT_DURATION);

    m_muted = uriNode.get_optional<Mute>(ResourcesXlf::attr(ResourcesXlf::Media::Audio::Mute));
    m_looped = uriNode.get_optional<Loop>(ResourcesXlf::attr(ResourcesXlf::Media::Audio::Loop));
    m_volume = uriNode.get_optional<int>(ResourcesXlf::attr(ResourcesXlf::Media::Audio::Volume));
}

boost::optional<AudioOptions::Mute> AudioOptions::muted() const
{
    return m_muted;
}

boost::optional<AudioOptions::Loop> AudioOptions::looped() const
{
    return m_looped;
}

boost::optional<int> AudioOptions::volume() const
{
    return m_volume;
}

std::istream& operator >>(std::istream& in, AudioOptions::Mute& mute)
{
    bool temp;
    in >> temp;
    mute = static_cast<AudioOptions::Mute>(temp);
    return in;
}

std::istream& operator >>(std::istream& in, AudioOptions::Loop& loop)
{
    bool temp;
    in >> temp;
    loop = static_cast<AudioOptions::Loop>(temp);
    return in;
}
