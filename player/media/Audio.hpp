#include "Media.hpp"

class FilePath;
class IAudioHandler;

class Audio : public Media, public IPlayable
{
public:
    Audio(int id, const FilePath& path, std::unique_ptr<IAudioHandler>&& handler);

    void play() override;
    void stop() override;

    void apply(MediaVisitor& visitor) override;
    void handleEvent(const Event& ev) override;

    void setVolume(int volume);
    void setLooped(bool looped);
    bool looped() const;

private:
    void onAudioFinished();

private:
    std::unique_ptr<IAudioHandler> m_handler;
    bool m_looped = false;

};
