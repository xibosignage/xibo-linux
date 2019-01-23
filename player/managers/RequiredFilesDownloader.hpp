#pragma once

#include "RequiredItems.hpp"
#include <boost/thread/future.hpp>

using DownloadFilesResults = std::vector<boost::future<void>>;

class RequiredFilesDownloader
{
public:
    boost::future<void> download(const RegularFiles& files);

private:
    DownloadFilesResults downloadAllFiles(const RegularFiles& files);
    boost::future<void> downloadFile(const RegularFile& file);
    boost::future<void> downloadHttpFile(const std::string& fileName, const std::string& fileUrl);
    boost::future<void> downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileUrl, std::size_t fileSize);
    void createFile(const std::string& fileName, const std::string& fileContent);

};
