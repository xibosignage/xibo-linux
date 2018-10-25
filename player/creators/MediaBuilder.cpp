#include "MediaBuilder.hpp"

#include "media/IMedia.hpp"
#include "utils/Resources.hpp"

std::unique_ptr<IMedia> MediaBuilder::build()
{
    auto media = doBuild();
    media->setDuration(m_duration);
    return media;
}

MediaBuilder& MediaBuilder::id(int id)
{
    m_id = id;
    return *this;
}

MediaBuilder& MediaBuilder::path(const boost::optional<std::string>& path)
{
    m_path = Resources::directory() / path.value();
    return *this;
}

MediaBuilder& MediaBuilder::duration(int duration)
{
    m_duration = duration;
    return *this;
}
