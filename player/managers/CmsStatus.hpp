#pragma once

#include "common/DateTime.hpp"

struct CmsStatus
{
    bool registered;
    DateTime lastChecked;
    size_t requiredFiles;
};
