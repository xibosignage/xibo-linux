#pragma once

#include "config/Resources.hpp"
#include "common/types/internal/WeakType.hpp"

struct Resource : WeakType<FilePath>
{
    Resource(const std::string& file) : WeakType(Resources::directory() / file) {}
};
