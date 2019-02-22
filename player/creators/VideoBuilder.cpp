#include "VideoBuilder.hpp"

#include "media/VideoHandler.hpp"

#include <boost/optional/optional.hpp>

std::unique_ptr<Video> VideoBuilder::create()
{
    return std::unique_ptr<Video>(new Video{m_id, m_width, m_height, m_path, createHandler()});
}

std::unique_ptr<IVideoHandler> VideoBuilder::createDefaultHandler()
{
    return std::make_unique<VideoHandler>();
}

void VideoBuilder::doMediaSetup(Video& video)
{
    video.setMuted(m_mute);
    video.setLooped(m_loop);
}

VideoBuilder& VideoBuilder::retrieveMediaOptions(const VideoOptions& opts)
{
    m_mute = getMuteOption(opts.muted());
    m_loop = getLoopOption(opts.looped());
    return *this;
}

VideoBuilder& VideoBuilder::width(int width)
{
    m_width = width;
    return *this;
}

VideoBuilder& VideoBuilder::height(int height)
{
    m_height = height;
    return *this;
}

VideoOptions::Mute VideoBuilder::getMuteOption(const boost::optional<VideoOptions::Mute>& muteOpt)
{
    return muteOpt.value_or(DEFAULT_VIDEO_MUTED);
}

VideoOptions::Loop VideoBuilder::getLoopOption(const boost::optional<VideoOptions::Loop>& loopOpt)
{
    return loopOpt.value_or(DEFAULT_VIDEO_LOOPED);
}

