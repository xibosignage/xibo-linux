#pragma once

#include "IFileSystemAdaptor.hpp"

class FileSystemAdaptor : public IFileSystemAdaptor
{
public:
    bool isRegularFile(const FilePath& path) override;
    bool exists(const FilePath& path) override;
    bool createDirectory(const FilePath& path) override;
    FilePath currentPath() const override;
    void writeToFile(const FilePath& path, const std::string& content) override;
};
