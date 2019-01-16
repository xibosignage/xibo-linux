#pragma once

class FilePath;

class IFileSystemAdaptor
{
public:
    virtual ~IFileSystemAdaptor() = default;
    virtual bool isRegularFile(const FilePath& path) const = 0;
};
