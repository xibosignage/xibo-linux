#pragma once

#include "utils/ResponseResult.hpp"
#include "utils/logger/Logging.hpp"
#include "xmds/MediaInventoryItem.hpp"

#include <boost/thread/future.hpp>

using DownloadResult = boost::future<bool>;
using DownloadResults = std::vector<DownloadResult>;
using ResponseContentResult = ResponseResult<std::string>;
class XmdsFileDownloader;
class XmdsRequestSender;

class RequiredFilesDownloader
{
public:
    RequiredFilesDownloader(XmdsRequestSender& xmdsRequestSender);
    ~RequiredFilesDownloader();

    template<typename RequiredFileType>
    boost::future<MediaInventoryItems> download(const FilesToDownload<RequiredFileType>& files)
    {
        return boost::async(boost::launch::async, [this, files = std::move(files)](){
            auto downloadResults = downloadAll(files);
            return retrieveDownloadResults(files, std::move(downloadResults));
        });
    }

private:
    template<typename RequiredFileType>
    DownloadResults downloadAll(const FilesToDownload<RequiredFileType>& requiredFiles)
    {
        DownloadResults results;

        for(auto&& file : requiredFiles)
        {
            Log::trace(file);

            if(!isFileInCache(file))
            {
                results.emplace_back(downloadRequiredFile(file));
            }
        }

        return results;
    }

    template<typename RequiredFileType>
    MediaInventoryItems retrieveDownloadResults(const FilesToDownload<RequiredFileType>& files, DownloadResults&& results)
    {
        MediaInventoryItems items;

        for(std::size_t i = 0; i != results.size(); ++i)
        {
            bool downloadComplete = results[i].get();
            auto&& downloadedFile = files[i];

            items.emplace_back(downloadedFile, downloadComplete);
        }

        return items;
    }

    bool isFileInCache(const RegularFile& file) const;
    bool isFileInCache(const ResourceFile& resource) const;

    DownloadResult downloadRequiredFile(const ResourceFile& res);
    DownloadResult downloadRequiredFile(const RegularFile& file);
    DownloadResult downloadHttpFile(const std::string& fileName, const std::string& fileUrl);
    DownloadResult downloadXmdsFile(int fileId, const std::string& fileName, const std::string& fileType, std::size_t fileSize);
    bool processDownloadedContent(const ResponseContentResult& result, const std::string& fileName);

private:
    XmdsRequestSender& m_xmdsRequestSender;
    std::unique_ptr<XmdsFileDownloader> m_xmdsFileDownloader;

};
