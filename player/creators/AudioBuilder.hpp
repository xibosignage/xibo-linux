#include "MediaBuilder.hpp"

class AudioBuilder : public MediaBuilder
{
public:
    AudioBuilder& muted(const boost::optional<bool>& muted);
    AudioBuilder& looped(const boost::optional<bool>& looped);
    AudioBuilder& volume(const boost::optional<int>& volume);

protected:
    std::unique_ptr<IMedia> doBuild() override;

private:
    int m_volume;
    bool m_looped;
    bool m_muted;

};
