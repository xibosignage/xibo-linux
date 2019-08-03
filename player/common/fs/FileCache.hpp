#pragma once

#include "common/fs/FilePath.hpp"

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>

class FileCache : private boost::noncopyable
{
public:
    void loadFrom(const FilePath& cacheFile);
    bool cachedWithHash(const std::string& file, const std::string& targetHash) const;
    bool cached(const std::string& file) const;
    void save(const std::string& file, const std::string& content);

private:
    void addToCache(const std::string& file, const std::string& hash);
    void loadFileHashes(const FilePath& cacheFile);
    void saveFileHashes(const FilePath& path);

private:
    boost::property_tree::ptree m_fileCache;
    FilePath m_cacheFile;

};
