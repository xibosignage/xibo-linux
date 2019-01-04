#include "FileSystemAdaptor.hpp"

bool FileSystemAdaptor::isRegularFile(FilePath path) const
{
    return std::filesystem::is_regular_file(path);
}
