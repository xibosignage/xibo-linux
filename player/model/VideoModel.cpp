#include "VideoModel.hpp"

VideoModel::VideoModel(const VideoOptions& options) :
    MediaModel(options)
{
    m_looped = static_cast<Loop>(options.looped);
    m_muted = static_cast<Mute>(options.muted);
}

VideoModel::Loop VideoModel::looped() const
{
    return m_looped;
}

VideoModel::Mute VideoModel::muted() const
{
    return m_muted;
}

void VideoModel::apply(IMediaVisitor& visitor)
{
    visitor.visit(shared_from_this());
}
