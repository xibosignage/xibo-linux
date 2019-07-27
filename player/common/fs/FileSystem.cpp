#include "FileSystem.hpp"
#include "FilePath.hpp"
#include "FileSystemAdaptor.hpp"

std::unique_ptr<IFileSystemAdaptor> FileSystem::fs_ = std::make_unique<FileSystemAdaptor>();

void FileSystem::setAdaptor(std::unique_ptr<IFileSystemAdaptor>&& fs)
{
    fs_ = std::move(fs);
}

bool FileSystem::isRegularFile(const FilePath& path)
{
    return fs_->isRegularFile(path);
}

bool FileSystem::exists(const FilePath& path)
{
    return fs_->exists(path);
}

bool FileSystem::createDirectory(const FilePath& path)
{
    return fs_->createDirectory(path);
}

FilePath FileSystem::currentPath()
{
    return fs_->currentPath();
}

void FileSystem::writeToFile(const FilePath& path, const std::string& content)
{
    return fs_->writeToFile(path, content);
}
