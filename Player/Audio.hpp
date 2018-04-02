#include "Media.hpp"

class AudioHandler;

class Audio : public Media
{
public:
    Audio(const Region& region, int id, int duration, const std::string& uri, bool muted, bool looped);

    bool muted() const;
    bool looped() const;

    void stop() override;
    void start() override;

private:
    bool m_muted;
    bool m_looped;

    //std::unique_ptr<AudioHandler> m_handler;

};
