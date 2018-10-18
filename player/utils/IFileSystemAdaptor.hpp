#pragma once

#include <filesystem>

using FilePath = std::filesystem::path;

class IFileSystemAdaptor
{
public:
    virtual ~IFileSystemAdaptor() = default;
    virtual void setResourcesDirectory(FilePath path) = 0;
    virtual FilePath resourcesDirectory() const = 0;
    virtual bool isRegularFile(FilePath path) const = 0;
};
