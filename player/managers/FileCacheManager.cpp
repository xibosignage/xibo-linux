#include "FileCacheManager.hpp"

#include "utils/Resources.hpp"
#include "common/FileSystem.hpp"
#include "common/Utils.hpp"

#include <fstream>

void FileCacheManager::loadCache(const FilePath& cacheFile)
{
    m_cacheFilePath = cacheFile;

    if(!FileSystem::exists(cacheFile)) return;

    std::ifstream in(cacheFile.string());
    std::string hash;
    while(in >> hash)
    {
        m_fileCache.emplace(hash);
    }
}

bool FileCacheManager::isFileInCache(const std::string& fileHash) const
{
    return m_fileCache.count(fileHash) != 0;
}

void FileCacheManager::saveFile(const std::string& fileName, const std::string& fileContent)
{
    auto filePath = Resources::resDirectory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;

    addFileToCache(Utils::md5hash(fileContent));
}

void FileCacheManager::addFileToCache(const std::string& fileHash)
{
    m_fileCache.emplace(fileHash);

    updateCache(m_cacheFilePath);
}

void FileCacheManager::updateCache(const FilePath& path)
{
    std::ofstream out(path.string());
    for(auto hash : m_fileCache)
    {
        out << hash << std::endl;
    }
}

