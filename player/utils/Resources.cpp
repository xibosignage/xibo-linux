#include "Resources.hpp"
#include "utils/Utilities.hpp"

FilePath Resources::directory_;

FilePath Resources::directory()
{
    return directory_;
}

void Resources::setDirectory(const FilePath& directory)
{
    directory_ = directory;
    if(!std::filesystem::exists(directory_))
    {
        bool result = std::filesystem::create_directory(directory_);
        if(!result)
        {
            throw std::runtime_error("Unable to create resources directory");
        }
    }
}
