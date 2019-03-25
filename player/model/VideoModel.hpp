#pragma once

#include "MediaModel.hpp"
#include "parsers/Options.hpp"

class VideoModel : public MediaModel, public std::enable_shared_from_this<VideoModel>
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

    VideoModel(const VideoOptions& options);

    void apply(IMediaVisitor& visitor) override;

    Loop looped() const;
    Mute muted() const;

private:
    Loop m_looped;
    Mute m_muted;

};
