#include "VideoBuilder.hpp"
#include "media/VideoHandler.hpp"

#include <boost/optional/optional.hpp>

std::unique_ptr<Video> VideoBuilder::build()
{
    auto video = createVideo();
    prepareCommonParams(*video);
    video->setMuted(m_mute);
    video->setLooped(m_loop);
    return video;
}

std::unique_ptr<Video> VideoBuilder::createVideo()
{
    return std::make_unique<Video>(m_id, m_width, m_height, m_path, createHandler());
}

std::unique_ptr<IVideoHandler> VideoBuilder::createHandler()
{
    return std::make_unique<VideoHandler>();
}

VideoBuilder& VideoBuilder::width(int width)
{
    m_width = width;
    return *this;
}

VideoBuilder& VideoBuilder::mediaOptions(const ResourcesXlf::VideoOptions& opts)
{
    m_mute = getMuteOption(opts.muted());
    m_loop = getLoopOption(opts.looped());
    return *this;
}

VideoBuilder& VideoBuilder::height(int height)
{
    m_height = height;
    return *this;
}

bool VideoBuilder::getMuteOption(const boost::optional<bool>& muteOpt)
{
    return muteOpt.value_or(DEFAULT_VIDEO_MUTED);
}

bool VideoBuilder::getLoopOption(const boost::optional<bool>& loopOpt)
{
    return loopOpt.value_or(DEFAULT_VIDEO_LOOPED);
}

