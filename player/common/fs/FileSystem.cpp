#include "FileSystem.hpp"
#include "FilePath.hpp"

#include <fstream>
#include <boost/filesystem.hpp>

bool FileSystem::isRegularFile(const FilePath& path)
{
    return boost::filesystem::is_regular_file(path);
}

bool FileSystem::exists(const FilePath& path)
{
    return boost::filesystem::exists(path);
}

bool FileSystem::remove(const FilePath& path)
{
    return boost::filesystem::remove(path);
}

bool FileSystem::createDirectory(const FilePath& path)
{
    return boost::filesystem::create_directory(path);
}

FilePath FileSystem::currentPath()
{
    return boost::filesystem::current_path();
}

void FileSystem::writeToFile(const FilePath& path, const std::string& content)
{
    std::ofstream out(path);

    out << content;
}
