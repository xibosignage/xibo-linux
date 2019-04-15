#pragma once

#include "common/FilePath.hpp"

class Resources
{
    static FilePath directory_;

public:
    static FilePath resDirectory();
    static void setDirectory(const FilePath& resDirectory);

};

