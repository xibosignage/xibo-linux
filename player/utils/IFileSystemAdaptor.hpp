#pragma once

#include "constants.hpp"

class IFileSystemAdaptor
{
public:
    virtual ~IFileSystemAdaptor() = default;
    virtual bool isRegularFile(FilePath path) const = 0;
};
