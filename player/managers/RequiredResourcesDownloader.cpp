#include "RequiredResourcesDownloader.hpp"

#include "xmds/XMDSManager.hpp"

#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"
#include "utils/Logger.hpp"

#include <fstream>

boost::future<void> RequiredResourcesDownloader::download(const ResourceFiles& resources)
{
    return boost::async(boost::launch::async, [=](){
        auto downloadResults = downloadAllResources(resources);

        for(auto&& result : downloadResults)
        {
            result.wait();
        }
    });
}

DownloadResourcesResults RequiredResourcesDownloader::downloadAllResources(const ResourceFiles& resources)
{
    DownloadResourcesResults results;

    for(auto&& resource : resources)
    {
        Log::trace("Layout ID: {} Region ID: {} Media ID: {}", resource.layoutId, resource.regionId, resource.mediaId);

        auto processResource = [this, resource](auto future){
            processDownloadedResource(resource.mediaId, future.get());
        };

        results.emplace_back(Utils::xmdsManager().getResource(resource.layoutId, resource.regionId, resource.mediaId).then(std::move(processResource)));
    }

    return results;
}

void RequiredResourcesDownloader::processDownloadedResource(int mediaId, ResponseResult<GetResource::Result> response)
{
    auto [error, result] = response;
    if(!error)
    {
        auto filename = createResource(mediaId, result.resource);

        Log::debug("[{}] Downloaded", filename);
    }
    else
    {
        Log::debug("[{}] Download error", error.message());
    }

}

std::string RequiredResourcesDownloader::createResource(int mediaId, const std::string& resourceContent)
{
    auto filename = std::to_string(mediaId) + ".html";

    std::ofstream out(Resources::directory() / filename);
    out << resourceContent;

    return filename;
}
