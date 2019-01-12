#include "RequiredResourcesDownloader.hpp"

#include "xmds/XMDSManager.hpp"

#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"
#include "utils/Logger.hpp"

#include <fstream>

std::future<void> RequiredResourcesDownloader::download(const ResourceFiles& resources)
{
    return std::async(std::launch::async, [=](){
        downloadAllResources(resources);
    });
}

void RequiredResourcesDownloader::downloadAllResources(const ResourceFiles& resources)
{
    for(auto&& resource : resources)
    {
        Log::trace("Layout ID: {} Region ID: {} Media ID: {}", resource.layoutId, resource.regionId, resource.mediaId);

        auto result = Utils::xmdsManager().getResource(resource.layoutId, resource.regionId, resource.mediaId);
        processDownloadedResource(resource.mediaId, result.get());
    }
}

void RequiredResourcesDownloader::processDownloadedResource(int mediaId, const GetResource::Result& response)
{
    auto filename = createResource(mediaId, response.resource);

    Log::debug("[{}] Downloaded", filename);
}

std::string RequiredResourcesDownloader::createResource(int mediaId, const std::string& resourceContent)
{
    auto filename = std::to_string(mediaId) + ".html";

    std::ofstream out(Resources::directory() / filename);
    out << resourceContent;

    return filename;
}
