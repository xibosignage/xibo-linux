#pragma once

#include "common/fs/Resources.hpp"
#include "common/types/WeakType.hpp"

struct ResourceFile : WeakType<FilePath>
{
    ResourceFile(const std::string& file) : WeakType(Resources::directory() / file) {}
};
