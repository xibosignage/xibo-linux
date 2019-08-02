#include "RequiredFilesDownloader.hpp"

#include "managers/FileCacheManager.hpp"

#include "networking/xmds/XmdsFileDownloader.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"
#include "networking/HttpClient.hpp"

#include "utils/Managers.hpp"
#include "utils/Resources.hpp"
#include "common/Utils.hpp"

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

        return processDownloadedContent<ResourceFile>(ResponseContentResult{error, result.resource}, fileName);
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
    return HttpClient::instance().get(fileUrl).then([this, fileName](boost::future<HttpResponseResult> future){
        return processDownloadedContent<RegularFile>(future.get(), fileName);
    });
}

DownloadResult RequiredFilesDownloader::downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileType, std::size_t fileSize)
{
    return m_xmdsFileDownloader->download(fileId, fileType, fileSize).then([this, fileName](boost::future<XmdsResponseResult> future){
        return processDownloadedContent<RegularFile>(future.get(), fileName);
    });
}

bool RequiredFilesDownloader::shouldBeDownloaded(const RegularFile& file) const
{
    return !Managers::fileManager().isFileInCache(file.name, file.md5);
}

bool RequiredFilesDownloader::shouldBeDownloaded(const ResourceFile&) const
{
    return true;
}

template<>
void RequiredFilesDownloader::saveFile<RegularFile>(const std::string& fileName, const std::string& fileContent)
{
    Managers::fileManager().saveFile(fileName, fileContent);
}

template<>
void RequiredFilesDownloader::saveFile<ResourceFile>(const std::string& fileName, const std::string& fileContent)
{
    if(!Managers::fileManager().isFileInCache(fileName, Utils::md5hash(fileContent)))
    {
        Managers::fileManager().saveFile(fileName, fileContent);
    }
}
