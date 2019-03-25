#include "AudioModel.hpp"

AudioModel::AudioModel(const AudioOptions& options) :
    MediaModel(options)
{
    m_looped = static_cast<Loop>(options.looped);
    m_muted = static_cast<Mute>(options.muted);
    m_volume = options.volume;
}

AudioModel::Loop AudioModel::looped() const
{
    return m_looped;
}

AudioModel::Mute AudioModel::muted() const
{
    return m_muted;
}

int AudioModel::volume() const
{
    return m_volume;
}

void AudioModel::apply(IMediaVisitor& visitor)
{
    visitor.visit(shared_from_this());
}
