#include "FileCacheImpl.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/fs/Resource.hpp"
#include "common/logger/Logging.hpp"

static XmlNode::path_type attributePath(const std::string& path)
{
    return XmlNode::path_type(path, '|');
}

const XmlNode::path_type ValidAttr{attributePath("valid")};
const XmlNode::path_type Md5Attr{attributePath("md5")};

void FileCacheImpl::loadFrom(const FilePath& cacheFile)
{
    cacheFile_ = cacheFile;
    loadFileHashes(cacheFile);
}

void FileCacheImpl::loadFileHashes(const FilePath& path)
{
    if (FileSystem::exists(path))
    {
        try
        {
            fileCache_ = Parsing::xmlFrom(path);
        }
        catch (std::exception&)
        {
            Log::error("Error while loading file cache");
        }
    }
}

bool FileCacheImpl::valid(const std::string& filename) const
{
    auto node = fileCache_.get_child_optional(attributePath(filename));

    return node.has_value() && node->get<bool>(ValidAttr);
}

bool FileCacheImpl::cached(const std::string& filename, const Md5Hash& hash) const
{
    auto node = fileCache_.get_child_optional(attributePath(filename));

    if (node)
    {
        Md5Hash savedHash{node->get<std::string>(Md5Attr)};
        return savedHash == hash;
    }

    return false;
}

void FileCacheImpl::save(const std::string& fileName, const std::string& fileContent, const Md5Hash& hash)
{
    Resource path{fileName};

    FileSystem::writeToFile(path, fileContent);

    addToCache(fileName, Md5Hash::fromString(fileContent), hash);
}

void FileCacheImpl::markAsInvalid(const std::string& filename)
{
    auto node = fileCache_.get_child_optional(attributePath(filename));

    if (node)
    {
        node->put(ValidAttr, false);
    }
}

void FileCacheImpl::addToCache(const std::string& filename, const Md5Hash& hash, const Md5Hash& target)
{
    XmlNode node;
    node.put(Md5Attr, hash);
    node.put(ValidAttr, hash == target);

    fileCache_.put_child(attributePath(filename), node);

    saveFileHashes(cacheFile_);
}

void FileCacheImpl::saveFileHashes(const FilePath& path)
{
    try
    {
        Parsing::xmlTreeToFile(path, fileCache_);
    }
    catch (std::exception&)
    {
        Log::error("[FileCache] Error while updating file cache");
    }
}
