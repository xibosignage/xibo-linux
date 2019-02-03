#include "MediaBuilder.hpp"

#include "media/IVideoHandler.hpp"
#include "media/Video.hpp"
#include "parsers/VideoOptions.hpp"

const bool DEFAULT_VIDEO_LOOPED = false;
const bool DEFAULT_VIDEO_MUTED = false;

class VideoBuilder : public BaseMediaBuilder<VideoBuilder, ResourcesXlf::VideoOptions>
{
public:
    std::unique_ptr<Video> build();

    VideoBuilder& width(int width);
    VideoBuilder& height(int height);

protected:
    virtual std::unique_ptr<IVideoHandler> createHandler();
    VideoBuilder& mediaOptions(const ResourcesXlf::VideoOptions& opts) override;

private:
    std::unique_ptr<Video> createVideo();

    bool getMuteOption(const boost::optional<bool>& muteOpt);
    bool getLoopOption(const boost::optional<bool>& loopOpt);

private:
    int m_width;
    int m_height;
    bool m_loop;
    bool m_mute;

};
