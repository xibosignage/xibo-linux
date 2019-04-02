#pragma once

#include "utils/FilePath.hpp"
#include "utils/IFileSystemAdaptor.hpp"

#include <boost/noncopyable.hpp>
#include <string>
#include <unordered_set>

class FileCacheManager : private boost::noncopyable
{
public:
    FileCacheManager(const FilePath& cacheFile);

    bool isFileInCache(const std::string& fileHash) const;
    void saveFile(const std::string& fileName, const std::string& fileContent);

private:
    void addFileToCache(const std::string& fileHash);
    std::string md5hash(std::string_view data);

    void loadCacheFromDrive(const FilePath& path);
    void updateCacheOnDrive(const FilePath& path);

private:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    std::unordered_set<std::string> m_fileCache;
    FilePath m_cacheFilePath;

};
