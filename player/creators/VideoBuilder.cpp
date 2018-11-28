#include "VideoBuilder.hpp"

#include "media/Video.hpp"
#include "media/VideoHandler.hpp"

#include <boost/optional/optional.hpp>

const bool DEFAULT_LOOPED = false;
const bool DEFAULT_MUTED = false;

std::unique_ptr<IMedia> VideoBuilder::doBuild()
{
    auto video = createVideo();
    video->setMuted(m_muted);
    video->setLooped(m_looped);
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

VideoBuilder& VideoBuilder::height(int height)
{
    m_height = height;
    return *this;
}

VideoBuilder& VideoBuilder::muted(const boost::optional<bool>& muted)
{
    m_muted = muted.value_or(DEFAULT_MUTED);
    return *this;
}

VideoBuilder& VideoBuilder::looped(const boost::optional<bool>& looped)
{
    m_looped = looped.value_or(DEFAULT_LOOPED);
    return *this;
}

