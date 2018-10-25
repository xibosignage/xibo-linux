#include "MediaBuilder.hpp"

class VideoBuilder : public MediaBuilder
{
public:
    VideoBuilder& width(int width);
    VideoBuilder& height(int height);
    VideoBuilder& muted(const boost::optional<bool>& muted);
    VideoBuilder& looped(const boost::optional<bool>& looped);

protected:
    std::unique_ptr<IMedia> doBuild() override;

private:
    int m_width;
    int m_height;
    bool m_looped;
    bool m_muted;

};
