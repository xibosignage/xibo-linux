#pragma once

#include "xmds/RequiredFiles.hpp"
#include "xmds/GetResource.hpp"

#include <future>

class RequiredResourcesDownloader
{
public:
    std::future<void> download(const ResourceFiles& resources);

private:
    void downloadAllResources(const ResourceFiles& resources);
    void processDownloadedResource(int mediaId, const GetResource::Response& response);
    std::string createResource(int mediaId, const std::string& resourceContent);

};
