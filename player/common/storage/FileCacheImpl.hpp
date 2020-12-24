#pragma once

#include "common/parsing/XmlFileLoaderSaver.hpp"
#include "common/storage/FileCache.hpp"

#include <boost/noncopyable.hpp>

class FileCacheImpl : public FileCache, public XmlDefaultFileLoader, private boost::noncopyable
{
public:
    void loadFrom(const FilePath& cacheFile) override;
    bool valid(const std::string& filename) const override;
    bool cached(const RegularFile& file) const override;
    bool cached(const ResourceFile& file) const override;
    bool cached(const std::string& filename, const Md5Hash& hash) const override;
    std::vector<std::string> cachedFiles() const override;
    std::vector<std::string> invalidFiles() const override;
    void save(const std::string& filename, const std::string& content, const Md5Hash& hash) override;
    void save(const std::string& filename, const std::string& content, const DateTime& lastUpdate) override;
    void markAsInvalid(const std::string& filename) override;

protected:
    DocVersionType currentVersion() const override;
    boost::optional<DocVersionType> documentVersion(const XmlNode& tree) const override;
    std::unique_ptr<XmlFileLoader> backwardCompatibleLoader(const DocVersionType& version) const override;

private:
    void addToCache(const std::string& filename, const Md5Hash& hash, const Md5Hash& target);
    void addToCache(const std::string& filename, const Md5Hash& hash, const DateTime& lastUpdate);
    void loadFileHashes(const FilePath& path);
    void saveFileHashes(const FilePath& path);

private:
    XmlNode fileCache_;
    FilePath cacheFile_;
};
