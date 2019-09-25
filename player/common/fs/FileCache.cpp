#include "FileCache.hpp"

#include "common/Utils.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/fs/Resources.hpp"

#include <boost/property_tree/xml_parser.hpp>

using ptree = boost::property_tree::ptree;

void FileCache::loadFrom(const FilePath& cacheFile)
{
    m_cacheFile = cacheFile;
    loadFileHashes(cacheFile);
}

void FileCache::loadFileHashes(const FilePath& path)
{
    if (FileSystem::exists(path))
    {
        boost::property_tree::read_xml(path, m_fileCache);
    }
}

bool FileCache::valid(const std::string& filename) const
{
    auto node = m_fileCache.get_child_optional(ptree::path_type(filename, '|'));

    return node.has_value() && node->get<bool>(ptree::path_type("valid", '|'));
}

bool FileCache::cached(const std::string& filename, const std::string& hash) const
{
    auto node = m_fileCache.get_child_optional(ptree::path_type(filename, '|'));

    return node.has_value() && node->get<std::string>(ptree::path_type("md5", '|')) == hash;
}

void FileCache::save(const std::string& fileName, const std::string& fileContent, const std::string& md5)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;

    addToCache(fileName, Utils::md5hash(fileContent), md5);
}

void FileCache::markAsInvalid(const std::string& filename)
{
    auto node = m_fileCache.get_child_optional(ptree::path_type(filename, '|'));

    if (node)
    {
        node->put(ptree::path_type("valid", '|'), false);
    }
}

void FileCache::addToCache(const std::string& filename, const std::string& hash, const std::string& target)
{
    ptree node;
    node.put(ptree::path_type("md5", '|'), hash);
    node.put(ptree::path_type("valid", '|'), hash == target);

    m_fileCache.put_child(ptree::path_type(filename, '|'), node);

    saveFileHashes(m_cacheFile);
}

void FileCache::saveFileHashes(const FilePath& path)
{
    boost::property_tree::write_xml(path, m_fileCache);
}
