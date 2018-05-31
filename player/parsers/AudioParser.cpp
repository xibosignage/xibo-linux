#include "AudioParser.hpp"
#include "media/Audio.hpp"

const int MAX_VOLUME = 100;

AudioParser::AudioParser(const xlf_node& attrs, const xlf_node& options) :
    m_attrs(attrs), m_options(options)
{
}

std::unique_ptr<Media> AudioParser::parse()
{
    int id = m_attrs.template get<int>("id");
    std::string uri = m_options.get<std::string>("uri");
    int duration = m_attrs.get<int>("duration");

    bool mute = m_options.get<bool>("mute", false);
    bool loop = m_options.get<bool>("loop", false);
    double volume = m_options.get<int>("volume", MAX_VOLUME) / static_cast<double>(MAX_VOLUME);

    return std::make_unique<Audio>(id, duration, uri, mute, loop, volume);
}
