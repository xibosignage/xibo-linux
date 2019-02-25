#pragma once

#include "Media.hpp"
#include "options/AudioOptions.hpp"

class Uri;
class IAudioHandler;

class Audio : public Media, public IPlayable
{
public:
    void play() override;
    void stop() override;

    void handleEvent(const Event& ev) override;

    void setVolume(int volume);
    void setLooped(AudioOptions::Loop looped);
    AudioOptions::Loop looped() const;

private:
    friend class AudioBuilder;

    Audio(int id, const Uri& uri, std::unique_ptr<IAudioHandler>&& handler);

    void onAudioFinished();

private:
    std::unique_ptr<IAudioHandler> m_handler;
    AudioOptions::Loop m_looped;

};
