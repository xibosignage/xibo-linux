#include "FileCache.hpp"

#include "common/fs/Resources.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/Utils.hpp"

#include <boost/property_tree/xml_parser.hpp>

using ptree = boost::property_tree::ptree;

void FileCache::loadFrom(const FilePath& cacheFile)
{
    m_cacheFile = cacheFile;
    loadFileHashes(cacheFile);
}

void FileCache::loadFileHashes(const FilePath& path)
{
    if(FileSystem::exists(path))
    {
        boost::property_tree::read_xml(path, m_fileCache);
    }
}

bool FileCache::cachedWithHash(const std::string& file, const std::string& targetHash) const
{
    auto hash = m_fileCache.get_optional<std::string>(ptree::path_type(file, '|'));

    return hash ? hash == targetHash : false;
}

bool FileCache::cached(const std::string& file) const
{
    auto hash = m_fileCache.get_optional<std::string>(ptree::path_type(file, '|'));

    return hash.has_value();
}

void FileCache::save(const std::string& fileName, const std::string& fileContent)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath);
    out << fileContent;

    addToCache(fileName, Utils::md5hash(fileContent));
}

void FileCache::addToCache(const std::string& name, const std::string& hash)
{
    m_fileCache.put(ptree::path_type(name, '|'), hash);

    saveFileHashes(m_cacheFile);
}

void FileCache::saveFileHashes(const FilePath& path)
{
    boost::property_tree::write_xml(path, m_fileCache);
}

