#pragma once

#include "xmds/RequiredFiles.hpp"

#include <future>

using DownloadFilesResults = std::vector<std::future<void>>;
class ResponseResult;

class RequiredFilesDownloader
{
public:
    std::future<void> download(const RegularFiles& files);

private:
    DownloadFilesResults downloadAllFiles(const RegularFiles& files);
    std::future<void> downloadFile(const std::string& filename, const std::string& path);
    void createFile(const std::string& fileName, const std::string& fileContent);

};
