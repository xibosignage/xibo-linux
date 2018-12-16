#include "RequiredFilesDownloader.hpp"

#include "HTTPDownloader.hpp"
#include "xmds/XMDSManager.hpp"

#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"
#include "utils/Utilities.hpp"
#include "utils/FilePath.hpp"

#include <fstream>

void RequiredFilesDownloader::download(const RegularFiles& files, const ResourceFiles& resources, DownloadFinished callback)
{
    m_downloadOverall = files.size() + resources.size();

    downloadAllFiles(files, [=](){

    });

    downloadAllResources(resources, [=](){

    });
}

// FIXME GetFile should be used in case of XMDS download type
void RequiredFilesDownloader::downloadAllFiles(const RegularFiles& files, DownloadFinished)
{
    for(auto&& file : files)
    {
        Utils::logger()->trace("File type: {} Id: {} Size: {}", static_cast<int>(file.fileType), file.id, file.size);
        Utils::logger()->trace("MD5: {} Filename: {} Download type: {}", file.md5, file.filename, static_cast<int>(file.downloadType));

        Utils::httpDownloader().download(file.filename, file.path, [=](const DownloadedFile& downloadedFile){
            if(!downloadedFile.downloadError)
            {
                Utils::logger()->debug("[{}] Downloaded", downloadedFile.name);
            }
            else
            {
                Utils::logger()->error("[{}] Download error: {}", downloadedFile.name, downloadedFile.downloadError.message());
            }
        });
    }
}

void RequiredFilesDownloader::downloadAllResources(const ResourceFiles& resources, DownloadFinished)
{
    for(auto&& resource : resources)
    {
        Utils::logger()->trace("layout_id: {} region_id: {} media_id: {}", resource.layoutId, resource.regionId, resource.mediaId);

        downloadResource(resource.layoutId, resource.regionId, resource.mediaId, [=](std::string filename){
            Utils::logger()->debug("[{}] Downloaded", filename);
        });
    }
}

void RequiredFilesDownloader::downloadResource(int layoutId, int regionId, int mediaId, ResourceDownloadCallback callback)
{
    Utils::xmdsManager().getResource(layoutId, regionId, mediaId, [=](const GetResource::Response& response){
        auto filename = Resources::directory() / (std::to_string(mediaId) + ".html");
        std::ofstream out(filename.string());
        out << response.resource;
        callback(filename.string());
    });
}
