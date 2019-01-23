#include "RequiredFilesDownloader.hpp"

#include "HTTPManager.hpp"
#include "xmds/XMDSManager.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/FilePath.hpp"
#include "utils/Resources.hpp"

#include <fstream>

boost::future<void> RequiredFilesDownloader::download(const RegularFiles& files)
{
    return boost::async(boost::launch::async, [=](){
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

boost::future<void> RequiredFilesDownloader::downloadFile(const RegularFile& file)
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

boost::future<void> RequiredFilesDownloader::downloadHttpFile(const std::string& fileName, const std::string& fileUrl)
{
    return Utils::httpManager().get(fileUrl).then([this, fileName](boost::future<HTTPResponseResult> future){
        auto [httpError, httpResult] = future.get();
        if(!httpError)
        {
            createFile(fileName, httpResult);
            Log::debug("[{}] Downloaded", fileName);
        }
        else
        {
            Log::error("[{}] Download error: {}", fileName, httpError.message());
        }
    });
}

#include <boost/beast/core/detail/base64.hpp>
const std::size_t DEFAULT_CHUNK_SIZE = 524288;

// FIXME
boost::future<void> RequiredFilesDownloader::downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileType, std::size_t fileSize)
{
    std::size_t fileOffset = 0;
    std::string fileContent;

    while(fileOffset < fileSize)
    {
        std::size_t chunkSize = fileOffset + DEFAULT_CHUNK_SIZE >= fileSize ? fileSize - fileOffset : DEFAULT_CHUNK_SIZE;

        auto [error, result] = Utils::xmdsManager().getFile(fileId, fileType, fileOffset, chunkSize).get();
        fileContent += boost::beast::detail::base64_decode(result.base64chunk);

        fileOffset += chunkSize;
    }

    Log::debug("[{}] Downloaded", fileName);
    createFile(fileName, fileContent);

    return boost::async([](){});
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
