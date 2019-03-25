#include "AudioParser.hpp"

#include "XlfResources.hpp"
#include "Validators.hpp"

const int DEFAULT_DURATION = 0;
const bool DEFAULT_AUDIO_MUTED = false;
const bool DEFAULT_AUDIO_LOOPED = false;

AudioParser::AudioParser(const xml_node& node) :
    MediaParser(node)
{
}

AudioOptions AudioParser::parse()
{
    auto options = baseOptions();
    auto muted = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Media::Audio::Mute), DEFAULT_AUDIO_MUTED);
    auto looped = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Media::Audio::Loop), DEFAULT_AUDIO_LOOPED);
    auto volume = node().get<int>(ResourcesXlf::option(ResourcesXlf::Media::Audio::Volume), MAX_VOLUME);

    return AudioOptions{options, muted, looped, volume};
}

AudioOptions AudioParser::parseNode()
{
    auto uriNode = node().get_child(ResourcesXlf::Media::Uri);

    auto id = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Audio::Id));
    auto uri = uriNode.get_value<std::string>();
    auto muted = uriNode.get<bool>(ResourcesXlf::attr(ResourcesXlf::Media::Audio::Mute), DEFAULT_AUDIO_MUTED);
    auto looped = uriNode.get<bool>(ResourcesXlf::attr(ResourcesXlf::Media::Audio::Loop), DEFAULT_AUDIO_LOOPED);
    auto volume = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Audio::Volume), MAX_VOLUME);

    return AudioOptions{{id, Validators::validateUri(uri), DEFAULT_DURATION}, muted, looped, volume};
}

