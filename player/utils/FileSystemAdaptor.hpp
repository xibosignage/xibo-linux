#pragma once

#include "IFileSystemAdaptor.hpp"

class FileSystemAdaptor : public IFileSystemAdaptor
{
public:
    bool isRegularFile(FilePath path) const override;

};
