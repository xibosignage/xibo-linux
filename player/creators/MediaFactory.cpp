#include "MediaFactory.hpp"

#include "media/Audio.hpp"
#include "media/IMedia.hpp"
#include "utils/Resources.hpp"

MediaFactory::MediaFactory(const xlf_node& parentNode, const xlf_node& mediaNode)
{
    m_parentNode = parentNode;
    m_attrs = mediaNode.get_child("<xmlattr>");
    m_options = mediaNode.get_child("options");
    m_audioNode = mediaNode.get_child_optional("audio");
}

std::unique_ptr<IMedia> MediaFactory::create()
{
    auto media = doCreate();
    media->attachMedia(createAudioFromNode(media->duration()));
    return media;
}

const xlf_node& MediaFactory::parentNode() const
{
    return m_parentNode;
}

const xlf_node& MediaFactory::attrs() const
{
    return m_attrs;
}

const xlf_node& MediaFactory::options() const
{
    return m_options;
}

std::unique_ptr<IMedia> MediaFactory::createAudioFromNode(int parentDuration)
{
    if(m_audioNode)
    {
        auto uriNode = m_audioNode.value().get_child("uri");
        auto attrs = uriNode.get_child("<xmlattr>");

//        int id = a ttrs.get<int>("mediaId");
        auto uri = Resources::directory() / uriNode.get_value<std::string>();
        bool mute = attrs.get<bool>("mute", false);
        bool loop = attrs.get<bool>("loop", false);
        double volume = attrs.get<int>("volume", MAX_VOLUME) / static_cast<double>(MAX_VOLUME);

        return std::make_unique<Audio>(parentDuration, uri.string(), mute, loop, volume);
    }
    return nullptr;
}
