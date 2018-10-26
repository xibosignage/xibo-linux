#include "Resources.hpp"

FilePath Resources::directory_;

FilePath Resources::directory()
{
    return directory_;
}

void Resources::setDirectory(const FilePath& directory)
{
    directory_ = directory;
}
