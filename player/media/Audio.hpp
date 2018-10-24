#include "Media.hpp"

#include "IAudioHandler.hpp"

class Audio : public Media<IInvisibleMedia>
{
public:
    Audio(const FilePath& path, std::unique_ptr<IAudioHandler>&& handler);
    void apply(MediaVisitor& visitor) override;

    void setVolume(int volume);
    void setLooped(bool looped);

protected:
    void doStop() override;
    void doStart() override;

private:
    void onAudioFinished();

private:
    std::unique_ptr<IAudioHandler> m_handler;
    bool m_looped = false;

};
