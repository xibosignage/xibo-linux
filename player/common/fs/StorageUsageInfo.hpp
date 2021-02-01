#pragma once

#include "common/fs/FilePath.hpp"

struct StorageUsageInfo
{
    StorageUsageInfo getFor(const FilePath& path);

    std::uintmax_t total;
    std::uintmax_t available;
};
