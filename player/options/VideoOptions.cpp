#include "VideoOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

VideoOptions::VideoOptions(int id,
                           boost::optional<std::string> path,
                           int duration,
                           boost::optional<Mute> muted,
                           boost::optional<Loop> looped) :
    MediaOptions(id, path, duration), m_muted(muted), m_looped(looped)
{
}

VideoOptions::VideoOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_muted = node.get_optional<Mute>(ResourcesXlf::option(ResourcesXlf::Media::Video::Mute));
    m_looped = node.get_optional<Loop>(ResourcesXlf::option(ResourcesXlf::Media::Video::Loop));
}

boost::optional<VideoOptions::Mute> VideoOptions::muted() const
{
    return m_muted;
}

boost::optional<VideoOptions::Loop> VideoOptions::looped() const
{
    return m_looped;
}

std::istream& operator >>(std::istream& in, VideoOptions::Mute& mute)
{
    bool temp;
    in >> temp;
    mute = static_cast<VideoOptions::Mute>(temp);
    return in;
}

std::istream& operator >>(std::istream& in, VideoOptions::Loop& loop)
{
    bool temp;
    in >> temp;
    loop = static_cast<VideoOptions::Loop>(temp);
    return in;
}
