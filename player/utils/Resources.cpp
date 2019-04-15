#include "Resources.hpp"

#include "common/FileSystem.hpp"

FilePath Resources::directory_;

FilePath Resources::resDirectory()
{
    return directory_;
}

void Resources::setDirectory(const FilePath& directory)
{
    directory_ = directory;

    if(!FileSystem::exists(directory))
    {
        bool result = FileSystem::createDirectory(directory_);
        if(!result)
        {
            throw std::runtime_error("Unable to create resources directory");
        }
    }
}
