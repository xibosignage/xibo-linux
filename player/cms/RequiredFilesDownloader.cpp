#include "RequiredFilesDownloader.hpp"

#include "common/fs/FileCache.hpp"
#include "networking/HttpClient.hpp"

#include "GetResourceCommand.hpp"
#include "XmdsFileDownloader.hpp"

RequiredFilesDownloader::RequiredFilesDownloader(const CmsSettings& settings, FileCache& fileCache) :
    fileCache_{fileCache},
    settings_{settings},
    xmdsFileDownloader_{std::make_unique<XmdsFileDownloader>(settings)}
{
}

RequiredFilesDownloader::~RequiredFilesDownloader() {}

void RequiredFilesDownloader::saveRegularFile(const std::string& filename,
                                              const std::string& content,
                                              const Md5Hash& hash)
{
    fileCache_.save(filename, content, hash);
}

void RequiredFilesDownloader::saveResourceFile(const std::string& filename, const std::string& content)
{
    auto hash = Md5Hash::fromString(content);
    if (!fileCache_.cached(filename, hash))
    {
        fileCache_.save(filename, content, hash);
    }
}

bool RequiredFilesDownloader::onRegularFileDownloaded(const ResponseContentResult& result,
                                                      const std::string& filename,
                                                      const Md5Hash& hash)
{
    auto [error, fileContent] = result;
    if (!error)
    {
        saveRegularFile(filename, fileContent, hash);

        Log::debug("[{}] Downloaded", filename);
        return true;
    }
    else
    {
        Log::error("[{}] Download error: {}", filename, error);
        return false;
    }
}

bool RequiredFilesDownloader::onResourceFileDownloaded(const ResponseContentResult& result, const std::string& filename)
{
    auto [error, fileContent] = result;
    if (!error)
    {
        saveResourceFile(filename, fileContent);

        Log::debug("[{}] Downloaded", filename);
        return true;
    }
    else
    {
        Log::error("[{}] Download error: {}", filename, error);
        return false;
    }
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const ResourceFile& res)
{
    auto getResource = GetResourceCommand::create(
        settings_.address(), settings_.key(), settings_.displayId(), res.layoutId, res.regionId, res.mediaId);

    return getResource->executeFuture().then([this, res](auto future) {
        auto fileName = std::to_string(res.mediaId) + ".html";
        auto [error, result] = future.get();

        return onResourceFileDownloaded(ResponseContentResult{error, result.resource}, fileName);
    });
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const RegularFile& file)
{
    if (file.downloadType == RegularFile::DownloadType::HTTP)
    {
        auto uri = Uri::fromString(file.url);
        return HttpClient::instance().get(uri).then([this, file](boost::future<HttpResponseResult> future) {
            return onRegularFileDownloaded(future.get(), file.name, Md5Hash{file.hash});
        });
    }
    else
    {
        return xmdsFileDownloader_->download(file.id, file.type, file.size)
            .then([this, file](boost::future<XmdsResponseResult> future) {
                return onRegularFileDownloaded(future.get(), file.name, Md5Hash{file.hash});
            });
    }
}

bool RequiredFilesDownloader::shouldBeDownloaded(const RegularFile& file) const
{
    return !fileCache_.valid(file.name) || !fileCache_.cached(file.name, Md5Hash{file.hash});
}

bool RequiredFilesDownloader::shouldBeDownloaded(const ResourceFile&) const
{
    return true;
}
