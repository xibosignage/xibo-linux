#include "VideoBuilder.hpp"

#include "media/Video.hpp"
#include "media/VideoHandler.hpp"

std::unique_ptr<IMedia> VideoBuilder::doBuild()
{
    auto video = std::make_unique<Video>(m_width, m_height, m_path, std::make_unique<VideoHandler>());
    video->setMuted(m_muted);
    video->setLooped(m_looped);
    return video;
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
    m_muted = muted.value_or(false);
    return *this;
}

VideoBuilder& VideoBuilder::looped(const boost::optional<bool>& looped)
{
    m_looped = looped.value_or(false);
    return *this;
}

