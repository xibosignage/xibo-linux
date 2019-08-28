#pragma once

#include "FilePath.hpp"

class IFileCache
{
public:
    virtual ~IFileCache() = default;

    virtual void loadFrom(const FilePath& cacheFile) = 0;
    virtual bool valid(const std::string& filename) const = 0;
    virtual bool cached(const std::string& filename, const std::string& hash) const = 0;
    virtual void markAsInvalid(const std::string& filename) = 0;
    virtual void save(const std::string& filename, const std::string& content, const std::string& md5) = 0;
};
