#include "RequiredFilesDownloader.hpp"

#include "cms/xmds/XmdsFileDownloader.hpp"
#include "cms/xmds/XmdsRequestSender.hpp"
#include "common/storage/FileCache.hpp"
#include "networking/HttpClient.hpp"

RequiredFilesDownloader::RequiredFilesDownloader(XmdsRequestSender& xmdsRequestSender, FileCache& fileCache) :
    xmdsRequestSender_{xmdsRequestSender},
    fileCache_{fileCache},
    xmdsFileDownloader_{std::make_unique<XmdsFileDownloader>(xmdsRequestSender)}
{
}

RequiredFilesDownloader::~RequiredFilesDownloader() {}

bool RequiredFilesDownloader::onRegularFileDownloaded(const ResponseContentResult& result, const RegularFile& file)
{
    auto [error, fileContent] = result;
    if (!error)
    {
        fileCache_.save(file.name(), fileContent, file.hash());

        Log::debug("[{}] Downloaded", file.name());
        return true;
    }
    else
    {
        Log::error("[{}] Download error: {}", file.name(), error);
        return false;
    }
}

bool RequiredFilesDownloader::onResourceFileDownloaded(const ResponseContentResult& result, const ResourceFile& file)
{
    auto [error, fileContent] = result;
    if (!error)
    {
        fileCache_.save(file.name(), fileContent, file.lastUpdate());

        Log::debug("[{}] Downloaded", file.name());
        return true;
    }
    else
    {
        Log::error("[{}] Download error: {}", file.name(), error);
        return false;
    }
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const ResourceFile& file)
{
    return xmdsRequestSender_.getResource(file.layoutId(), file.regionId(), file.mediaId())
        .then([this, file](auto future) {
            auto [error, result] = future.get();

            return onResourceFileDownloaded(ResponseContentResult{error, result.resource}, file);
        });
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const RegularFile& file)
{
    if (file.downloadType() == RegularFile::DownloadType::HTTP)
    {
        auto uri = Uri::fromString(file.url());
        return HttpClient::instance().get(uri).then([this, file](boost::future<HttpResponseResult> future) {
            return onRegularFileDownloaded(future.get(), file);
        });
    }
    else
    {
        return xmdsFileDownloader_->download(file.id(), file.type(), file.size())
            .then([this, file](boost::future<XmdsResponseResult> future) {
                return onRegularFileDownloaded(future.get(), file);
            });
    }
}

bool RequiredFilesDownloader::shouldBeDownloaded(const RegularFile& file) const
{
    return !fileCache_.valid(file.name()) || !fileCache_.cached(file);
}

bool RequiredFilesDownloader::shouldBeDownloaded(const ResourceFile& file) const
{
    return !fileCache_.valid(file.name()) || !fileCache_.cached(file);
}
