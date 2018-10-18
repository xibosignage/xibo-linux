#include "FileSystemAdaptor.hpp"

void FileSystemAdaptor::setResourcesDirectory(FilePath path)
{
    m_resourcesDirectory = path;
}

FilePath FileSystemAdaptor::resourcesDirectory() const
{
    return m_resourcesDirectory;
}

bool FileSystemAdaptor::isRegularFile(FilePath path) const
{
    return std::filesystem::is_regular_file(path);
}
