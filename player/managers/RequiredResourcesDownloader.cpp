#include "RequiredResourcesDownloader.hpp"

#include "xmds/XMDSManager.hpp"

#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"
#include "utils/Utilities.hpp"
#include "utils/FilePath.hpp"

#include <fstream>

void RequiredResourcesDownloader::download(const ResourceFiles& resources, ResourceDownloadFinished callback)
{
    m_resourcesDownloaded.connect([=](){
        callback(5);
    });

    downloadAllResources(resources);
}

void RequiredResourcesDownloader::downloadAllResources(const ResourceFiles& resources)
{
    for(auto&& resource : resources)
    {
        Utils::logger()->trace("Layout ID: {} Region ID: {} Media ID: {}", resource.layoutId, resource.regionId, resource.mediaId);

        downloadResource(resource.layoutId, resource.regionId, resource.mediaId, [=](std::string resource)
        {
            processDownloadedResource(resource);
            if(m_downloadsCount == resources.size())
            {
                m_resourcesDownloaded.emit();
            }
        });
    }
}

void RequiredResourcesDownloader::downloadResource(int layoutId, int regionId, int mediaId, ResourceDownloadCallback callback)
{
    Utils::xmdsManager().getResource(layoutId, regionId, mediaId, [=](const GetResource::Response& response)
    {
        auto filename = createResource(mediaId, response.resource);
        callback(filename);
    });
}

std::string RequiredResourcesDownloader::createResource(int mediaId, const std::string& resourceContent)
{
    auto filename = Resources::directory() / (std::to_string(mediaId) + ".html");

    std::ofstream out(filename.string());
    out << resourceContent;

    return filename;
}

void RequiredResourcesDownloader::processDownloadedResource(std::string resource)
{
    Utils::logger()->debug("[{}] Downloaded", resource);
    ++m_downloadsCount;
}
