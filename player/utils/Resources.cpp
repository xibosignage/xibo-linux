#include "Resources.hpp"
#include "FilePath.hpp"

#include <filesystem>

FilePath Resources::directory_;

FilePath Resources::directory()
{
    return directory_;
}

void Resources::removeDirectoryContents(const FilePath& directory)
{
    if(std::filesystem::exists(directory))
    {
        std::filesystem::remove_all(directory);
    }
}

void Resources::setDirectory(const FilePath& directory)
{
    directory_ = directory;

    if(!std::filesystem::exists(directory))
    {
        bool result = std::filesystem::create_directory(directory_);
        if(!result)
        {
            throw std::runtime_error("Unable to create resources directory");
        }
    }
}
