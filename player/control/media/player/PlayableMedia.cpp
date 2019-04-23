#include "PlayableMedia.hpp"

#include "control/media/Media.hpp"
#include "constants.hpp"

PlayableMedia::PlayableMedia(const MediaPlayerOptions& options, std::unique_ptr<MediaPlayer>&& player) :
    Media(options),
    m_player(std::move(player))
{
    m_player->setVolume(options.muted == MediaPlayerOptions::Mute::Enable ? MIN_VOLUME : MAX_VOLUME);
    m_player->load(options.uri);

    mediaFinished().connect([this](){
        m_player->stopPlayback();
    });

    started().connect([this](){
        m_player->play();
    });

    stopped().connect([this](){
        m_player->stopAndRemove();
    });

    m_player->playbackFinished().connect([this, options](){
        if(options.duration == 0)
        {
            mediaFinished().emit();
            return;
        }

        if(options.looped == MediaPlayerOptions::Loop::Enable)
        {
            m_player->play();
        }
    });
}
