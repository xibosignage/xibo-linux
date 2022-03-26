#pragma once

#include "common/parsing/XmlDefaultFileHandler.hpp"
#include "common/storage/FileCache.hpp"

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

class FileCacheImpl : public FileCache, public XmlDefaultFileHandler, private boost::noncopyable
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
    XmlDocVersion currentVersion() const override;
    NodePath versionAttributePath() const override;
    std::unique_ptr<XmlFileLoader> backwardCompatibleLoader(const XmlDocVersion& version) const override;

private:
    void addToCache(const std::string& filename, const Md5Hash& hash, const Md5Hash& target);
    void addToCache(const std::string& filename, const Md5Hash& hash, const DateTime& lastUpdate);
    void loadFileHashes(const FilePath& path);
    void saveFileHashes(const FilePath& path);

private:
    boost::mutex fileCacheMutex_;
    XmlNode fileCache_;
    FilePath cacheFile_;
};
