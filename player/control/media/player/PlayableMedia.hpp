#pragma once

#include "MediaPlayerOptions.hpp"
#include "MediaPlayer.hpp"
#include "control/media/Media.hpp"

class PlayableMedia : public Media
{
public:
    PlayableMedia(const MediaPlayerOptions& options, std::unique_ptr<MediaPlayer>&& player);

private:
    std::unique_ptr<MediaPlayer> m_player;

};
