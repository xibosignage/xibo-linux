#include "RequiredFilesDownloader.hpp"

#include "HTTPDownloader.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/FilePath.hpp"

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
        Log::trace("MD5: {} File name: {} Download type: {}", file.md5, file.filename, static_cast<int>(file.downloadType));

        results.push_back(downloadFile(file.filename, file.path));
    }

    return results;
}

std::future<void> RequiredFilesDownloader::downloadFile(const std::string& filename, const std::string& path)
{
    // FIXME
    return Utils::httpDownloader().download(filename, path, [=](const DownloadedFile& file){
        processDownloadedFile(file);
    });
}

void RequiredFilesDownloader::processDownloadedFile(const DownloadedFile& file)
{
    if(!file.downloadError)
    {
        Log::debug("[{}] Downloaded", file.name);
    }
    else
    {
        Log::error("[{}] Download error: {}", file.name, file.downloadError.message());
    }
}
