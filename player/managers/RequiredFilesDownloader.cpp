#include "RequiredFilesDownloader.hpp"

#include "HTTPManager.hpp"
#include "xmds/XMDSManager.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/FilePath.hpp"
#include "utils/Resources.hpp"

#include <fstream>

std::future<void> RequiredFilesDownloader::download(const RegularFiles& files)
{
    return std::async(std::launch::async, [=](){
        auto downloadResults = downloadAllFiles(files);

        for(auto&& result : downloadResults)
        {
            result.wait();
        }
    });
}

DownloadFilesResults RequiredFilesDownloader::downloadAllFiles(const RegularFiles& files)
{
    DownloadFilesResults results;

    for(auto&& file : files)
    {
        Log::trace("File type: {} ID: {} Size: {}", file.type, file.id, file.size);
        Log::trace("MD5: {} File name: {} Download type: {}", file.md5, file.name, static_cast<int>(file.downloadType));

        results.push_back(downloadFile(file));
    }

    return results;
}

std::future<void> RequiredFilesDownloader::downloadFile(const RegularFile& file)
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

std::future<void> RequiredFilesDownloader::downloadHttpFile(const std::string& fileName, const std::string& fileUrl)
{
    return Utils::httpManager().get(fileUrl, [=](const ResponseResult& response){
        if(!response.error)
        {
            createFile(fileName, response.result);
            Log::debug("[{}] Downloaded", fileName);
        }
        else
        {
            Log::error("[{}] Download error: {}", fileName, response.error.message());
        }
    });
}

#include <boost/beast/core/detail/base64.hpp>

std::future<void> RequiredFilesDownloader::downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileType, std::size_t size)
{
    auto result = Utils::xmdsManager().getFile(fileId, fileType, 0, size);
    Log::debug("downloaded file {} {}", fileName, size);
    createFile(fileName, boost::beast::detail::base64_decode(result.get().base64chunk));

    return std::async(std::launch::async, [](){});
}

void RequiredFilesDownloader::createFile(const std::string& fileName, const std::string& fileContent)
{
    auto filePath = Resources::directory() / fileName;

    // FIXME it should use FileCacheManager
    if(!std::filesystem::exists(filePath.string()))
    {
        std::ofstream out(filePath.string());
        out << fileContent;
    }
}
