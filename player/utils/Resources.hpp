#pragma once

#include "constants.hpp"

class Resources
{
    static FilePath directory_;

public:
    static FilePath directory();
    static void setDirectory(const FilePath& directory);

};

