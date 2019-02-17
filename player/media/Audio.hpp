#pragma once

#include "Media.hpp"

class FilePath;
class IAudioHandler;

class Audio : public Media, public IPlayable
{
public:
    void play() override;
    void stop() override;

    void handleEvent(const Event& ev) override;

    void setVolume(int volume);
    void setLooped(bool looped);
    bool looped() const;

private:
    friend class AudioBuilder;

    Audio(int id, const FilePath& path, std::unique_ptr<IAudioHandler>&& handler);

    void onAudioFinished();

private:
    std::unique_ptr<IAudioHandler> m_handler;
    bool m_looped = false;

};
