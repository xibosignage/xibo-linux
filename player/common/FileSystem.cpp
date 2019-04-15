#include "FileSystem.hpp"
#include "FilePath.hpp"

bool FileSystem::isRegularFile(const FilePath& path)
{
    return std::filesystem::is_regular_file(path);
}

bool FileSystem::exists(const FilePath& path)
{
    return std::filesystem::exists(path);
}

bool FileSystem::createDirectory(const FilePath& path)
{
    return std::filesystem::create_directory(path);
}

FilePath FileSystem::currentPath()
{
    return std::filesystem::current_path();
}
