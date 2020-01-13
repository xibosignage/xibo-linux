#pragma once

#include "common/types/internal/WeakType.hpp"
#include "config/AppConfig.hpp"

struct Resource : WeakType<FilePath>
{
    Resource(const std::string& file) : WeakType(AppConfig::resourceDirectory() / file) {}
};
