#pragma once

#include "utils/FilePath.hpp"

#include <boost/noncopyable.hpp>
#include <string>
#include <unordered_set>

class FileCacheManager : private boost::noncopyable
{
public:
    FileCacheManager();

    bool isFileInCache(const std::string& fileHash) const;
    void addFileToCache(const std::string& fileHash);
    void saveFile(const std::string& fileName, const std::string& fileContent);

private:
    void loadCacheFromDrive(const FilePath& path);
    void updateCacheOnDrive(const FilePath& path);

private:
    std::unordered_set<std::string> m_fileCache;
    FilePath m_cacheFilePath;

};
