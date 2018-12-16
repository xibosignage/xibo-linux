#pragma once

#include "xmds/RequiredFiles.hpp"

using DownloadFinished = std::function<void()>;
using ResourceDownloadCallback = std::function<void(std::string)>;

class RequiredFilesDownloader
{
public:    
    void download(const RegularFiles& files, const ResourceFiles& resources, DownloadFinished callback);

private:
    void downloadAllFiles(const RegularFiles& files, DownloadFinished callback);
    void downloadAllResources(const ResourceFiles& resources, DownloadFinished callback);

    void downloadResource(int layoutId, int regionId, int mediaId, ResourceDownloadCallback callback);

private:
    size_t m_downloadCount = 0;
    size_t m_downloadOverall = 0;

};
