#include "FileCacheManager.hpp"

#include "utils/Resources.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/Utils.hpp"

#include <boost/property_tree/xml_parser.hpp>

using ptree = boost::property_tree::ptree;

void FileCacheManager::loadCache(const FilePath& cacheFile)
{
    m_cacheFilePath = cacheFile;
    loadFileHashes(cacheFile);
}

void FileCacheManager::loadFileHashes(const FilePath& path)
{
    if(FileSystem::exists(path))
    {
        boost::property_tree::read_xml(path, m_fileCache);
    }
}

bool FileCacheManager::isFileInCache(const std::string& fileName, const std::string& fileHash) const
{
    auto hash = m_fileCache.get_optional<std::string>(ptree::path_type(fileName, '|'));

    return hash ? hash == fileHash : false;
}

void FileCacheManager::saveFile(const std::string& fileName, const std::string& fileContent)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;

    addFileToCache(fileName, Utils::md5hash(fileContent));
}

void FileCacheManager::addFileToCache(const std::string& fileName, const std::string& fileHash)
{
    m_fileCache.put(ptree::path_type(fileName, '|'), fileHash);

    saveFileHashes(m_cacheFilePath);
}

void FileCacheManager::saveFileHashes(const FilePath& path)
{
    boost::property_tree::write_xml(path, m_fileCache);
}

