#pragma once

#include "IFileCache.hpp"

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>

class FileCache : public IFileCache, private boost::noncopyable
{
public:
    void loadFrom(const FilePath& cacheFile) override;
    bool cachedWithHash(const std::string& file, const std::string& targetHash) const override;
    bool cached(const std::string& file) const override;
    void save(const std::string& file, const std::string& content) override;

private:
    void addToCache(const std::string& file, const std::string& hash);
    void loadFileHashes(const FilePath& cacheFile);
    void saveFileHashes(const FilePath& path);

private:
    boost::property_tree::ptree m_fileCache;
    FilePath m_cacheFile;

};
