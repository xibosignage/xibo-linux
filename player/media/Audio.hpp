#include "Media.hpp"

#include "adaptors/IAudioHandler.hpp"

class Audio : public Media<IInvisibleMedia>
{
public:
    Audio(bool looped, std::unique_ptr<IAudioHandler>&& handler);
    void apply(MediaVisitor& visitor) override;

protected:
    void doStop() override;
    void doStart() override;

private:
    void onAudioFinished();

private:
    std::unique_ptr<IAudioHandler> m_handler;
    bool m_looped;

};
