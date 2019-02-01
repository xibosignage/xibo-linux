#include "FileCacheManager.hpp"

#include "utils/Resources.hpp"

#include <fstream>
#include <openssl/md5.h>
#include <boost/format.hpp>

const std::string DEFAULT_CACHE_FILE = "cachedFiles.txt";

FileCacheManager::FileCacheManager() :
    m_cacheFilePath(Resources::directory() / DEFAULT_CACHE_FILE)
{
    loadCacheFromDrive(m_cacheFilePath);
}

bool FileCacheManager::isFileInCache(const std::string& fileHash) const
{
    return m_fileCache.count(fileHash) != 0;
}

void FileCacheManager::saveFile(const std::string& fileName, const std::string& fileContent)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;

    addFileToCache(md5hash(fileContent));
}

std::string FileCacheManager::md5hash(std::string_view data)
{
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.data()), data.size(), result);

    std::stringstream stream;
    for(unsigned char byte : result)
    {
        stream << boost::format("%02x") % static_cast<short>(byte);
    }
    return stream.str();
}

void FileCacheManager::addFileToCache(const std::string& fileHash)
{
    m_fileCache.emplace(fileHash);

    updateCacheOnDrive(m_cacheFilePath);
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

