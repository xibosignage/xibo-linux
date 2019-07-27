#include "FileSystemAdaptor.hpp"
#include "FilePath.hpp"

#include <filesystem>
#include <fstream>

bool FileSystemAdaptor::isRegularFile(const FilePath& path)
{
    return std::filesystem::is_regular_file(path);
}

bool FileSystemAdaptor::exists(const FilePath& path)
{
    return std::filesystem::exists(path);
}

bool FileSystemAdaptor::createDirectory(const FilePath& path)
{
    return std::filesystem::create_directory(path);
}

FilePath FileSystemAdaptor::currentPath() const
{
    return std::filesystem::current_path();
}

void FileSystemAdaptor::writeToFile(const FilePath& path, const std::string& content)
{
    std::ofstream out(path.string());

    out << content;
}
