#pragma once

#include <string>

class FilePath;

class IFileSystemAdaptor
{
public:
    virtual ~IFileSystemAdaptor() = default;

    virtual bool isRegularFile(const FilePath& path) = 0;
    virtual bool exists(const FilePath& path) = 0;
    virtual bool createDirectory(const FilePath& path) = 0;
    virtual FilePath currentPath() const = 0;
    virtual void writeToFile(const FilePath& path, const std::string& content) = 0;
};
