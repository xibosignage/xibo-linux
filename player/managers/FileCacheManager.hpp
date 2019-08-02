#pragma once

#include "common/fs/FilePath.hpp"

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>

class FileCacheManager : private boost::noncopyable
{
public:
    void cacheFrom(const FilePath& cacheFile);
    bool isFileInCache(const std::string& fileName, const std::string& fileHash) const;
    void saveFile(const std::string& fileName, const std::string& fileContent);

private:
    void loadFileHashes(const FilePath& cacheFile);
    void addFileToCache(const std::string& fileName, const std::string& fileHash);
    std::string md5hash(std::string_view data);

    void saveFileHashes(const FilePath& path);

private:
    boost::property_tree::ptree m_fileCache;
    FilePath m_cacheFilePath;

};
