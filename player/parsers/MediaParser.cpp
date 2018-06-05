#include "MediaParser.hpp"
#include "media/Audio.hpp"
#include "utils/utilities.hpp"
#include "constants.hpp"

MediaParser::MediaParser(const xlf_node& parent_node, const xlf_node& media_node) :
    m_parent_node(parent_node)
{
    m_attrs = media_node.get_child("<xmlattr>");
    m_options = media_node.get_child("options");
    m_audio_node = media_node.get_child_optional("audio");
}

const xlf_node& MediaParser::attrs() const
{
    return m_attrs;
}

const xlf_node& MediaParser::options() const
{
    return m_options;
}

const xlf_node& MediaParser::parent_node() const
{
    return m_parent_node;
}

// FIXME: шаблонный метод parse + virtual doParse (чтобы избежать attach_media)
std::unique_ptr<Media> MediaParser::parse_audio_node()
{
    if(m_audio_node)
    {
        auto uri_node = m_audio_node.value().get_child("uri");
        auto attrs = uri_node.get_child("<xmlattr>");

        int id = attrs.get<int>("mediaId");
        std::string uri = utilities::example_dir() + "/" + uri_node.get_value<std::string>();
        bool mute = attrs.get<bool>("mute", false);
        bool loop = attrs.get<bool>("loop", false);
        double volume = attrs.get<int>("volume", MAX_VOLUME) / static_cast<double>(MAX_VOLUME);

        return std::make_unique<Audio>(id, 0, uri, mute, loop, volume);
    }
    return nullptr;
}
