#include "FileSystemAdaptor.hpp"
#include "FilePath.hpp"

bool FileSystemAdaptor::isRegularFile(const FilePath& path) const
{
    return std::filesystem::is_regular_file(path);
}
