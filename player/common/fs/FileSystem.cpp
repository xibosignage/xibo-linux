#include "FileSystem.hpp"

#include "common/fs/FilePath.hpp"
#include "common/fs/StorageUsageInfo.hpp"

#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>

namespace fs = boost::filesystem;

bool FileSystem::isRegularFile(const FilePath& path)
{
    return fs::is_regular_file(path);
}

bool FileSystem::isDirectory(const FilePath& path)
{
    return fs::is_directory(path);
}

void FileSystem::move(const FilePath& source, const FilePath& dest)
{
    if (exists(source))
    {
        copy(source, dest);
        removeAll(source);
    }
}

void FileSystem::copy(const FilePath& source, const FilePath& dest)
{
    fs::copy(source, dest);
}

bool FileSystem::exists(const FilePath& path)
{
    return fs::exists(path);
}

bool FileSystem::empty(const FilePath& path)
{
    return fs::is_empty(path);
}

bool FileSystem::remove(const FilePath& path)
{
    return fs::remove(path);
}

bool FileSystem::removeAll(const FilePath& path)
{
    return fs::remove_all(path);
}

bool FileSystem::createDirectory(const FilePath& path)
{
    return fs::create_directory(path);
}

FilePath FileSystem::currentPath()
{
    return fs::current_path();
}

std::string FileSystem::readFromFile(const FilePath& path)
{
    std::ifstream in{path.string()};

    std::stringstream buffer;
    buffer << in.rdbuf();

    return buffer.str();
}

void FileSystem::writeToFile(const FilePath& path, const std::string& content)
{
    std::ofstream out(path.string());

    out << content;
}

StorageUsageInfo FileSystem::storageUsageFor(const FilePath& path)
{
    auto spaceInfo = fs::space(path);
    return StorageUsageInfo{spaceInfo.capacity, spaceInfo.available};
}
