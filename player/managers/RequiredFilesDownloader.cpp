#include "RequiredFilesDownloader.hpp"

#include "HttpManager.hpp"
#include "FileCacheManager.hpp"

#include "xmds/XmdsFileDownloader.hpp"
#include "xmds/XmdsRequestSender.hpp"

#include "utils/Resources.hpp"

#include <fstream>

RequiredFilesDownloader::RequiredFilesDownloader(XmdsRequestSender& xmdsRequestSender) :
    m_xmdsRequestSender(xmdsRequestSender),
    m_xmdsFileDownloader(std::make_unique<XmdsFileDownloader>(xmdsRequestSender))
{
}

RequiredFilesDownloader::~RequiredFilesDownloader()
{
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const ResourceFile& res)
{
    return m_xmdsRequestSender.getResource(res.layoutId, res.regionId, res.mediaId).then([this, res](auto future){

        auto fileName = std::to_string(res.mediaId) + ".html";
        auto [error, result] = future.get();

        return processDownloadedContent(ResponseContentResult{error, result.resource}, fileName);
    });
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const RegularFile& file)
{
    if(file.downloadType == DownloadType::HTTP)
    {
        return downloadHttpFile(file.name, file.url);
    }
    else
    {
        return downloadXmdsFile(file.id, file.name, file.type, file.size);
    }
}

DownloadResult RequiredFilesDownloader::downloadHttpFile(const std::string& fileName, const std::string& fileUrl)
{
    return Utils::httpManager().get(fileUrl).then([this, fileName](boost::future<HttpResponseResult> future){
        return processDownloadedContent(future.get(), fileName);
    });
}

DownloadResult RequiredFilesDownloader::downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileType, std::size_t fileSize)
{
    return m_xmdsFileDownloader->download(fileId, fileType, fileSize).then([this, fileName](boost::future<XmdsResponseResult> future){
        return processDownloadedContent(future.get(), fileName);
    });
}

bool RequiredFilesDownloader::processDownloadedContent(const ResponseContentResult& result, const std::string& fileName)
{
    auto [error, fileContent] = result;
    if(!error)
    {
        Utils::fileManager().saveFile(fileName, fileContent);

        Log::debug("[{}] Downloaded", fileName);
        return true;
    }
    else
    {
        Log::error("[{}] Download error: {}", fileName, error);
        return false;
    }
}

bool RequiredFilesDownloader::isFileInCache(const RegularFile& file) const
{
    return Utils::fileManager().isFileInCache(file.md5);
}

bool RequiredFilesDownloader::isFileInCache(const ResourceFile&) const
{
    return false;
}
