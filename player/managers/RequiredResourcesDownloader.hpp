#pragma once

#include "xmds/RequiredFiles.hpp"

#include <sigc++/sigc++.h>

using ResourceDownloadFinished = std::function<void(int)>;
using ResourceDownloadCallback = std::function<void(std::string)>;

class RequiredResourcesDownloader
{
public:
    void download(const ResourceFiles& resources, ResourceDownloadFinished callback);

private:
    void downloadAllResources(const ResourceFiles& resources);
    void downloadResource(int layoutId, int regionId, int mediaId, ResourceDownloadCallback callback);
    void processDownloadedResource(std::string resource);
    std::string createResource(int mediaId, const std::string& resourceContent);

private:
    sigc::signal<void()> m_resourcesDownloaded;
    size_t m_downloadsCount = 0;

};
