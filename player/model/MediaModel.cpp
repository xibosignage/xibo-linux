#include "MediaModel.hpp"

#include "parsers/Validators.hpp"

MediaModel::MediaModel(const MediaOptions& options) :
    m_id(options.id), m_duration(options.duration), m_uri(options.uri)
{
}

void MediaModel::attachAudio(const std::shared_ptr<AudioModel>& media)
{
    m_attachedMedia = media;
}

std::shared_ptr<AudioModel> MediaModel::attachedAudio() const
{
    return m_attachedMedia;
}

int MediaModel::id() const
{
    return m_id;
}

int MediaModel::duration() const
{
    return m_duration;
}

Uri MediaModel::uri() const
{
    return m_uri;
}
