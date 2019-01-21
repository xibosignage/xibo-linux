#include "RequiredFilesDownloader.hpp"

#include "HTTPManager.hpp"

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
        Log::trace("File type: {} ID: {} Size: {}", static_cast<int>(file.fileType), file.id, file.size);
        Log::trace("MD5: {} File name: {} Download type: {}", file.md5, file.name, static_cast<int>(file.downloadType));


        results.push_back(downloadFile(file.name, file.path));
    }

    return results;
}

std::future<void> RequiredFilesDownloader::downloadFile(const std::string& fileName, const std::string& path)
{
    return Utils::httpManager().send(path, [=](const ResponseResult& response){
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
