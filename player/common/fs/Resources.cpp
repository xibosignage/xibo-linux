#include "Resources.hpp"

#include "common/fs/FileSystem.hpp"

FilePath Resources::directory_;

FilePath Resources::directory()
{
    return directory_;
}

void Resources::setDirectory(const FilePath& directory)
{
    directory_ = directory;

    if (!FileSystem::exists(directory))
    {
        bool result = FileSystem::createDirectory(directory_);
        if (!result)
        {
            throw std::runtime_error("Unable to create resources directory");
        }
    }
}
