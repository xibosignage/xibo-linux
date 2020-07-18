#pragma once

#include "common/fs/FilePath.hpp"
#include "common/crypto/Md5Hash.hpp"

class FileCache
{
public:
    virtual ~FileCache() = default;

    virtual void loadFrom(const FilePath& cacheFile) = 0;
    virtual bool valid(const std::string& filename) const = 0;
    virtual bool cached(const std::string& filename, const Md5Hash& hash) const = 0;
    virtual void markAsInvalid(const std::string& filename) = 0;
    virtual void save(const std::string& filename, const std::string& content, const Md5Hash& hash) = 0;
};
