#pragma once

#include "FilePath.hpp"

class Resources
{
    static FilePath directory_;

public:
    static FilePath directory();
    static void removeDirectoryContents(const FilePath& directory);
    static void setDirectory(const FilePath& directory);

};

