#pragma once

#include "RequiredItems.hpp"
#include "xmds/GetResource.hpp"

#include <boost/thread/future.hpp>

#include "xmds/SOAPManager.hpp"

using DownloadResourcesResults = std::vector<boost::future<void>>;

class RequiredResourcesDownloader
{
public:
    boost::future<void> download(const ResourceFiles& resources);

private:
    DownloadResourcesResults downloadAllResources(const ResourceFiles& resources);
    void processDownloadedResource(int mediaId, ResponseResult<GetResource::Result> response);
    std::string createResource(int mediaId, const std::string& resourceContent);

};
