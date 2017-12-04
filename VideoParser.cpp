#include "VideoParser.hpp"

#include "Video.hpp"

std::shared_ptr<Video> VideoParser::create_from_attrs(const boost::property_tree::ptree& attrs)
{
    bool mute = attrs.get<bool>("mute", false);
    bool loop = attrs.get<bool>("loop", false);

    return std::make_shared<Video>(m_id, m_duration, m_render, m_uri, mute, loop);
}
