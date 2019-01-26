#include "FileCacheManager.hpp"

#include "utils/Resources.hpp"

#include <fstream>

const std::string DEFAULT_CACHE_FILE = "requiredFiles.txt";

FileCacheManager::FileCacheManager() :
    m_cacheFilePath(Resources::directory() / DEFAULT_CACHE_FILE)
{
    loadCacheFromDrive(m_cacheFilePath);
}

bool FileCacheManager::isFileInCache(const std::string& fileHash) const
{
    return m_fileCache.count(fileHash) != 0;
}

void FileCacheManager::addFileToCache(const std::string& fileHash)
{
    m_fileCache.emplace(fileHash);

    updateCacheOnDrive(m_cacheFilePath);
}

void FileCacheManager::saveFile(const std::string& fileName, const std::string& fileContent)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;
}

void FileCacheManager::loadCacheFromDrive(const FilePath& path)
{
    if(!std::filesystem::exists(path)) return;

    std::ifstream in(path.string());
    std::string hash;
    while(in >> hash)
    {
        m_fileCache.emplace(hash);
    }
}

void FileCacheManager::updateCacheOnDrive(const FilePath& path)
{
    std::ofstream out(path.string());
    for(auto hash : m_fileCache)
    {
        out << hash << std::endl;
    }
}
