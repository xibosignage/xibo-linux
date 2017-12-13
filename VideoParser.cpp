#include "VideoParser.hpp"

ParsedVideo VideoParser::parse()
{
    bool muted = m_options.get<bool>("mute", false);
    bool looped = m_options.get<bool>("loop", false);

    return ParsedVideo(m_id, m_duration, m_uri, muted, looped);
}
