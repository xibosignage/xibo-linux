#pragma once

#include "FilePath.hpp"

class IFileCache
{
public:
    virtual ~IFileCache() = default;

    virtual void loadFrom(const FilePath& cacheFile) = 0;
    virtual bool cachedWithHash(const std::string& file, const std::string& targetHash) const = 0;
    virtual bool cached(const std::string& file) const = 0;
    virtual void save(const std::string& file, const std::string& content) = 0;
};
