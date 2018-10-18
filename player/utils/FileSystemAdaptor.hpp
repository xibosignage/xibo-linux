#pragma once

#include "IFileSystemAdaptor.hpp"

class FileSystemAdaptor : public IFileSystemAdaptor
{
public:
    void setResourcesDirectory(FilePath path) override;
    FilePath resourcesDirectory() const override;
    bool isRegularFile(FilePath path) const override;

private:
    FilePath m_resourcesDirectory;

};
