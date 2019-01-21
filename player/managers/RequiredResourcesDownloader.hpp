#pragma once

#include "RequiredItems.hpp"
#include "xmds/GetResource.hpp"

#include <future>

class RequiredResourcesDownloader
{
public:
    std::future<void> download(const ResourceFiles& resources);

private:
    void downloadAllResources(const ResourceFiles& resources);
    void processDownloadedResource(int mediaId, const GetResource::Result& response);
    std::string createResource(int mediaId, const std::string& resourceContent);

};
