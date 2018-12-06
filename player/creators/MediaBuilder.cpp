#include "MediaBuilder.hpp"

#include "media/IMedia.hpp"
#include "utils/Resources.hpp"
#include "utils/FileSystemAdaptor.hpp"

#include <boost/optional/optional.hpp>

MediaBuilder::MediaBuilder()
{
    m_filesystem = std::make_unique<FileSystemAdaptor>();
}

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
    if(path)
    {
        auto fullPath = Resources::directory() / path.value();

        checkPath(fullPath);

        m_path = fullPath;
    }
    return *this;
}

void MediaBuilder::checkPath(FilePath path)
{
    if(!filesystem().isRegularFile(path))
        throw std::runtime_error("Not valid path");
}

MediaBuilder& MediaBuilder::duration(int duration)
{
    m_duration = duration;
    return *this;
}

IFileSystemAdaptor& MediaBuilder::filesystem()
{
    return *m_filesystem;
}
