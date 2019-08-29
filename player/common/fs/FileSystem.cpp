#include "FileSystem.hpp"
#include "FilePath.hpp"

#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

bool FileSystem::isRegularFile(const FilePath& path)
{
    return fs::is_regular_file(path);
}

bool FileSystem::isDirectory(const FilePath& path)
{
    return fs::is_directory(path);
}

void FileSystem::copyRecursive(const FilePath& source, const FilePath& dest, const std::set<FilePath>& exceptions)
{
    if(!isDirectory(source) || !isDirectory(dest) || empty(source)) return;

    for(fs::directory_iterator file{source}; file != fs::directory_iterator{}; ++file)
    {
        fs::path current(file->path());
        if(exceptions.count(current) > 0) continue;

        if(fs::is_directory(current))
        {
            copyRecursive(current, dest / current.filename());
        }
        else
        {
            fs::copy_file(current, dest / current.filename());
        }
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

void FileSystem::writeToFile(const FilePath& path, const std::string& content)
{
    std::ofstream out(path);

    out << content;
}
