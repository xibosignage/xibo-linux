#pragma once

#include "common/fs/FilePath.hpp"

class Resources
{
    static FilePath directory_;

public:
    static FilePath directory();
    static void setDirectory(const FilePath& directory);
};
