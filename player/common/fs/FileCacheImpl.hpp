#pragma once

#include "common/Parsing.hpp"
#include "common/fs/FileCache.hpp"

#include <boost/noncopyable.hpp>

class FileCacheImpl : public FileCache, private boost::noncopyable
{
public:
    void loadFrom(const FilePath& cacheFile) override;
    bool valid(const std::string& filename) const override;
    bool cached(const std::string& filename, const Md5Hash& hash) const override;
    void save(const std::string& filename, const std::string& content, const Md5Hash& hash) override;
    void markAsInvalid(const std::string& filename) override;

private:
    void addToCache(const std::string& filename, const Md5Hash& hash, const Md5Hash& target);
    void loadFileHashes(const FilePath& cacheFile);
    void saveFileHashes(const FilePath& path);

private:
    XmlNode fileCache_;
    FilePath cacheFile_;
};
