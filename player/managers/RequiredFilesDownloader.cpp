#include "RequiredFilesDownloader.hpp"

#include "networking/HttpClient.hpp"
#include "networking/xmds/XmdsFileDownloader.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"

#include "common/Utils.hpp"
#include "common/fs/FileCache.hpp"
#include "common/fs/Resources.hpp"
#include "utils/Managers.hpp"

RequiredFilesDownloader::RequiredFilesDownloader(XmdsRequestSender& xmdsRequestSender) :
    m_xmdsRequestSender(xmdsRequestSender),
    m_xmdsFileDownloader(std::make_unique<XmdsFileDownloader>(xmdsRequestSender))
{
}

RequiredFilesDownloader::~RequiredFilesDownloader() {}

void RequiredFilesDownloader::saveRegularFile(const std::string& filename, const std::string& content,
                                              const std::string& hash)
{
    Managers::fileManager().save(filename, content, hash);
}

void RequiredFilesDownloader::saveResourceFile(const std::string& filename, const std::string& content)
{
    std::string hash = Utils::md5hash(content);
    if (!Managers::fileManager().cached(filename, hash))
    {
        Managers::fileManager().save(filename, content, hash);
    }
}

bool RequiredFilesDownloader::onRegularFileDownloaded(const ResponseContentResult& result, const std::string& filename,
                                                      const std::string& hash)
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
    return m_xmdsRequestSender.getResource(res.layoutId, res.regionId, res.mediaId).then([this, res](auto future) {
        auto fileName = std::to_string(res.mediaId) + ".html";
        auto [error, result] = future.get();

        return onResourceFileDownloaded(ResponseContentResult{error, result.resource}, fileName);
    });
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const RegularFile& file)
{
    if (file.downloadType == RegularFile::DownloadType::HTTP)
    {
        return HttpClient::instance().get(file.url).then([this, file](boost::future<HttpResponseResult> future) {
            return onRegularFileDownloaded(future.get(), file.name, file.hash);
        });
    }
    else
    {
        return m_xmdsFileDownloader->download(file.id, file.type, file.size)
            .then([this, file](boost::future<XmdsResponseResult> future) {
                return onRegularFileDownloaded(future.get(), file.name, file.hash);
            });
    }
}

bool RequiredFilesDownloader::shouldBeDownloaded(const RegularFile& file) const
{
    return !Managers::fileManager().valid(file.name) || !Managers::fileManager().cached(file.name, file.hash);
}

bool RequiredFilesDownloader::shouldBeDownloaded(const ResourceFile&) const
{
    return true;
}
