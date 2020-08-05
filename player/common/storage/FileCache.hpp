#pragma once

#include "common/fs/FilePath.hpp"
#include "common/storage/RequiredItems.hpp"

class FileCache
{
public:
    virtual ~FileCache() = default;

    virtual void loadFrom(const FilePath& cacheFile) = 0;
    virtual bool valid(const std::string& filename) const = 0;
    virtual bool cached(const RegularFile& file) const = 0;
    virtual bool cached(const ResourceFile& file) const = 0;
    virtual bool cached(const std::string& filename, const Md5Hash& hash) const = 0;
    virtual std::vector<std::string> cachedFiles() const = 0;
    virtual std::vector<std::string> invalidFiles() const = 0;
    virtual void markAsInvalid(const std::string& filename) = 0;
    virtual void save(const std::string& filename, const std::string& content, const Md5Hash& hash) = 0;
    virtual void save(const std::string& filename, const std::string& content, const DateTime& lastUpdate) = 0;
};
