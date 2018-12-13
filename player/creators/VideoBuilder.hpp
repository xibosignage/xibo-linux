#include "MediaBuilder.hpp"

#include "media/IVideoHandler.hpp"
#include "media/Video.hpp"

const bool DEFAULT_VIDEO_LOOPED = false;
const bool DEFAULT_VIDEO_MUTED = false;

class VideoBuilder : public BaseMediaBuilder<VideoBuilder>
{
public:
    std::unique_ptr<Video> build();

    VideoBuilder& width(int width);
    VideoBuilder& height(int height);
    VideoBuilder& muted(const boost::optional<bool>& muted);
    VideoBuilder& looped(const boost::optional<bool>& looped);

protected:
    virtual std::unique_ptr<IVideoHandler> createHandler();

private:
    std::unique_ptr<Video> createVideo();

private:
    int m_width;
    int m_height;
    bool m_looped;
    bool m_muted;

};
