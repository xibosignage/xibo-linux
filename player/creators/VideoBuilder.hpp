#include "MediaBuilder.hpp"

#include "media/VideoHandler.hpp"
#include "media/Video.hpp"
#include "parsers/VideoOptions.hpp"

const bool DEFAULT_VIDEO_LOOPED = false;
const bool DEFAULT_VIDEO_MUTED = false;

class VideoBuilder;

template<>
struct BuilderTraits<VideoBuilder>
{
    using Component = Video;
    using DefaultHandler = VideoHandler;
    using Options = ResourcesXlf::VideoOptions;
};

class VideoBuilder : public AbstractMediaBuilder<VideoBuilder>
{
public:
    VideoBuilder& width(int width);
    VideoBuilder& height(int height);

protected:
    VideoBuilder& retrieveMediaOptions(const ResourcesXlf::VideoOptions& opts) override;
    std::unique_ptr<Video> create() override;
    void doMediaSetup(Video& video) override;

private:
    bool getMuteOption(const boost::optional<bool>& muteOpt);
    bool getLoopOption(const boost::optional<bool>& loopOpt);

private:
    int m_width;
    int m_height;
    bool m_loop;
    bool m_mute;

};
