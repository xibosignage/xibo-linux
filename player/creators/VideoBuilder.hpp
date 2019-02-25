#include "AbstractMediaBuilder.hpp"

#include "media/IVideoHandler.hpp"
#include "media/Video.hpp"
#include "options/VideoOptions.hpp"

const VideoOptions::Mute DEFAULT_VIDEO_MUTED = VideoOptions::Mute::Disable;
const VideoOptions::Loop DEFAULT_VIDEO_LOOPED = VideoOptions::Loop::Disable;

class VideoBuilder;

template<>
struct BuilderTraits<VideoBuilder>
{
    using Component = Video;
    using Handler = IVideoHandler;
    using Options = VideoOptions;
};

class VideoBuilder : public AbstractMediaBuilder<VideoBuilder>
{
public:
    VideoBuilder& width(int width);
    VideoBuilder& height(int height);

protected:
    void retrieveMediaOptions(const VideoOptions& opts) override;
    std::unique_ptr<Video> create() override;
    std::unique_ptr<IVideoHandler> createDefaultHandler() override;
    void doMediaSetup(Video& video) override;

private:
    VideoOptions::Mute getMuteOption(const boost::optional<VideoOptions::Mute>& muteOpt);
    VideoOptions::Loop getLoopOption(const boost::optional<VideoOptions::Loop>& loopOpt);

private:
    int m_width;
    int m_height;
    VideoOptions::Mute m_mute;
    VideoOptions::Loop m_loop;

};
