#include "FileCacheImpl.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/fs/Resources.hpp"

#include <fstream>

void FileCacheImpl::loadFrom(const FilePath& cacheFile)
{
    cacheFile_ = cacheFile;
    loadFileHashes(cacheFile);
}

void FileCacheImpl::loadFileHashes(const FilePath& path)
{
    if (FileSystem::exists(path))
    {
        fileCache_ = Parsing::xmlFrom(path);
    }
}

bool FileCacheImpl::valid(const std::string& filename) const
{
    auto node = fileCache_.get_child_optional(XmlNode::path_type(filename, '|'));

    return node.has_value() && node->get<bool>(XmlNode::path_type("valid", '|'));
}

bool FileCacheImpl::cached(const std::string& filename, const Md5Hash& hash) const
{
    auto node = fileCache_.get_child_optional(XmlNode::path_type(filename, '|'));

    if (node)
    {
        Md5Hash savedHash{node->get<std::string>(XmlNode::path_type("md5", '|'))};
        return savedHash == hash;
    }

    return false;
}

void FileCacheImpl::save(const std::string& fileName, const std::string& fileContent, const Md5Hash& hash)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;

    addToCache(fileName, Md5Hash::fromString(fileContent), hash);
}

void FileCacheImpl::markAsInvalid(const std::string& filename)
{
    auto node = fileCache_.get_child_optional(XmlNode::path_type(filename, '|'));

    if (node)
    {
        node->put(XmlNode::path_type("valid", '|'), false);
    }
}

void FileCacheImpl::addToCache(const std::string& filename, const Md5Hash& hash, const Md5Hash& target)
{
    XmlNode node;
    node.put(XmlNode::path_type("md5", '|'), hash);
    node.put(XmlNode::path_type("valid", '|'), hash == target);

    fileCache_.put_child(XmlNode::path_type(filename, '|'), node);

    saveFileHashes(cacheFile_);
}

void FileCacheImpl::saveFileHashes(const FilePath& path)
{
    Parsing::xmlTreeToFile(path, fileCache_);
}
