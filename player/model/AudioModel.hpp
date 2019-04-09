#pragma once

#include "MediaModel.hpp"
#include "parsers/Options.hpp"

class AudioModel : public MediaModel, public std::enable_shared_from_this<AudioModel>
{
public:
    enum class Mute
    {
        Disable,
        Enable
    };

    enum class Loop
    {
        Disable,
        Enable
    };

    AudioModel(const AudioOptions& options);

    void apply(IMediaVisitor& visitor) override;

    Loop looped() const;
    Mute muted() const;
    int volume() const;

private:
    Loop m_looped;
    Mute m_muted;
    int m_volume;

};
