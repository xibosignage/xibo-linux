#include "MediaParser.hpp"
#include "media/Audio.hpp"
#include "utils/utilities.hpp"

MediaParser::MediaParser(const xlf_node& parentNode, const xlf_node& mediaNode) :
    m_parentNode(parentNode)
{
    m_attrs = mediaNode.get_child("<xmlattr>");
    m_options = mediaNode.get_child("options");
    m_audioNode = mediaNode.get_child_optional("audio");
}

std::unique_ptr<Media> MediaParser::parse()
{
    auto media = doParse();
    media->attachAudio(parseAudioNode(media->duration()));
    return media;
}

const xlf_node& MediaParser::attrs() const
{
    return m_attrs;
}

const xlf_node& MediaParser::options() const
{
    return m_options;
}

const xlf_node& MediaParser::parentNode() const
{
    return m_parentNode;
}

std::unique_ptr<Media> MediaParser::parseAudioNode(int parentDuration)
{
    if(m_audioNode)
    {
        auto uriNode = m_audioNode.value().get_child("uri");
        auto attrs = uriNode.get_child("<xmlattr>");

        int id = attrs.get<int>("mediaId");
        auto uri = utils::resourcesDir() / uriNode.get_value<std::string>();
        bool mute = attrs.get<bool>("mute", false);
        bool loop = attrs.get<bool>("loop", false);
        double volume = attrs.get<int>("volume", MAX_VOLUME) / static_cast<double>(MAX_VOLUME);

        return std::make_unique<Audio>(id, parentDuration, uri.string(), mute, loop, volume);
    }
    return nullptr;
}
