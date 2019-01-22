#pragma once

#include "RequiredItems.hpp"

#include <future>

using DownloadFilesResults = std::vector<std::future<void>>;
class ResponseResult;

class RequiredFilesDownloader
{
public:
    std::future<void> download(const RegularFiles& files);

private:
    DownloadFilesResults downloadAllFiles(const RegularFiles& files);
    std::future<void> downloadFile(const RegularFile& file);
    std::future<void> downloadHttpFile(const std::string& fileName, const std::string& fileUrl);
    std::future<void> downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileUrl, std::size_t size);
    void createFile(const std::string& fileName, const std::string& fileContent);

};
