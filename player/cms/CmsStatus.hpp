#pragma once

#include "common/dt/DateTime.hpp"

struct CmsStatus
{
    bool registered;
    DateTime lastChecked;
    size_t requiredFiles;
};
