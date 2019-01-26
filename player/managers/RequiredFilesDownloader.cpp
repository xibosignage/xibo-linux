#include "RequiredFilesDownloader.hpp"

#include "HTTPManager.hpp"
#include "XMDSDownloader.hpp"
#include "xmds/XMDSManager.hpp"

#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"

#include <fstream>

RequiredFilesDownloader::RequiredFilesDownloader() :
    m_xmdsDownloader(std::make_unique<XMDSDownloader>())
{
}

RequiredFilesDownloader::~RequiredFilesDownloader()
{
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const ResourceFile& res)
{
    return Utils::xmdsManager().getResource(res.layoutId, res.regionId, res.mediaId).then([this, res](auto future){

        auto fileName = std::to_string(res.mediaId) + ".html";
        auto [error, result] = future.get();

        return processDownloadedContent(ResponseContentResult{error, result.resource}, fileName);
    });
}

DownloadResult RequiredFilesDownloader::downloadRequiredFile(const RegularFile& file)
{
    // FIXME it should use FileCacheManager

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
    return Utils::httpManager().get(fileUrl).then([this, fileName](boost::future<HTTPResponseResult> future){
        return processDownloadedContent(future.get(), fileName);
    });
}

DownloadResult RequiredFilesDownloader::downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileType, std::size_t fileSize)
{
    return m_xmdsDownloader->download(fileId, fileType, fileSize).then([this, fileName](boost::future<XMDSResponseResult> future){
        return processDownloadedContent(future.get(), fileName);
    });
}

bool RequiredFilesDownloader::processDownloadedContent(const ResponseContentResult& result, const std::string& fileName)
{
    auto [error, fileContent] = result;
    if(!error)
    {
        createFile(fileName, fileContent);
        Log::debug("[{}] Downloaded", fileName);
        return true;
    }
    else
    {
        Log::debug("[{}] Download error: ", fileName, error);
        return false;
    }
}

void RequiredFilesDownloader::createFile(const std::string& fileName, const std::string& fileContent)
{
    auto filePath = Resources::directory() / fileName;

    std::ofstream out(filePath.string());
    out << fileContent;
}
