#include "MediaPlayerController.hpp"

#include "control/media/Media.hpp"
#include "constants.hpp"

MediaPlayerController::MediaPlayerController(const MediaPlayerOptions& options, Media& media, std::unique_ptr<MediaPlayer>&& player) :
    m_media(media),
    m_player(std::move(player))
{
    m_player->setVolume(options.muted == MediaPlayerOptions::Mute::Enable ? MIN_VOLUME : MAX_VOLUME);
    m_player->load(options.uri);

    m_media.mediaFinished().connect([this](){
        m_player->stop();
    });

    m_media.started().connect([this](){
        m_player->play();
    });

    m_media.stopped().connect([this](){
        m_player->stop();
    });

    m_player->playbackFinished().connect([this, options](){
        if(options.duration == 0)
        {
            m_media.mediaFinished().emit();
            return;
        }

        if(options.looped == MediaPlayerOptions::Loop::Enable)
        {
            m_player->play();
        }
    });
}
