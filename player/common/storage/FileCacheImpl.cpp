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
const XmlNode::path_type LastUpdateAttr{attributePath("updated")};

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

bool FileCacheImpl::cached(const RegularFile& file) const
{
    return cached(file.name(), file.hash());
}

bool FileCacheImpl::cached(const ResourceFile& file) const
{
    auto node = fileCache_.get_child_optional(attributePath(file.name()));

    if (node)
    {
        auto lastUpdate = node->get_optional<int>(LastUpdateAttr);
        if (lastUpdate)
        {
            auto savedLastUpdated = DateTime::utcFromTimestamp(*lastUpdate);
            return savedLastUpdated >= file.lastUpdate();
        }
    }

    return false;
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

std::vector<std::string> FileCacheImpl::cachedFiles() const
{
    std::vector<std::string> files;
    for (auto&& [name, node] : fileCache_)
    {
        files.push_back(name);
    }
    return files;
}

std::vector<std::string> FileCacheImpl::invalidFiles() const
{
    std::vector<std::string> files;
    for (auto&& [name, node] : fileCache_)
    {
        if (!valid(name))
        {
            files.push_back(name);
        }
    }
    return files;
}

void FileCacheImpl::save(const std::string& fileName, const std::string& fileContent, const Md5Hash& hash)
{
    Resource path{fileName};

    FileSystem::writeToFile(path, fileContent);

    addToCache(fileName, Md5Hash::fromString(fileContent), hash);
}

void FileCacheImpl::save(const std::string& fileName, const std::string& fileContent, const DateTime& lastUpdate)
{
    Resource path{fileName};

    FileSystem::writeToFile(path, fileContent);

    addToCache(fileName, Md5Hash::fromString(fileContent), lastUpdate);
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

void FileCacheImpl::addToCache(const std::string& filename, const Md5Hash& hash, const DateTime& lastUpdate)
{
    XmlNode node;
    node.put(Md5Attr, hash);
    node.put(LastUpdateAttr, lastUpdate.timestamp());
    node.put(ValidAttr, true);

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
