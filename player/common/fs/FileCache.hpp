#pragma once

#include "IFileCache.hpp"

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>

class FileCache : public IFileCache, private boost::noncopyable
{
public:
    void loadFrom(const FilePath& cacheFile) override;
    bool valid(const std::string& filename) const override;
    bool cached(const std::string& filename, const std::string& hash) const override;
    void save(const std::string& filename, const std::string& content, const std::string& md5) override;
    void markAsInvalid(const std::string& filename) override;

private:
    void addToCache(const std::string& filename, const std::string& hash, const std::string& target);
    void loadFileHashes(const FilePath& cacheFile);
    void saveFileHashes(const FilePath& path);

private:
    boost::property_tree::ptree m_fileCache;
    FilePath m_cacheFile;
};
