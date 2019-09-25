#pragma once

#include "common/logger/Logging.hpp"
#include "networking/ResponseResult.hpp"
#include "networking/xmds/MediaInventoryItem.hpp"

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

    template <typename RequiredFileType>
    boost::future<MediaInventoryItems> download(const FilesToDownload<RequiredFileType>& files)
    {
        return boost::async(boost::launch::async, [this, files = std::move(files)]() {
            auto downloadResults = downloadAll(files);
            return retrieveDownloadResults(files, std::move(downloadResults));
        });
    }

private:
    template <typename RequiredFileType>
    DownloadResults downloadAll(const FilesToDownload<RequiredFileType>& requiredFiles)
    {
        DownloadResults results;

        for (auto&& file : requiredFiles)
        {
            Log::trace(file);

            if (shouldBeDownloaded(file))
            {
                results.emplace_back(downloadRequiredFile(file));
            }
        }

        return results;
    }

    template <typename RequiredFileType>
    MediaInventoryItems retrieveDownloadResults(const FilesToDownload<RequiredFileType>& files,
                                                DownloadResults&& results)
    {
        MediaInventoryItems items;

        for (std::size_t i = 0; i != results.size(); ++i)
        {
            bool downloadComplete = results[i].get();
            auto&& downloadedFile = files[i];

            items.emplace_back(downloadedFile, downloadComplete);
        }

        return items;
    }

    void saveRegularFile(const std::string& filename, const std::string& content, const std::string& hash);
    void saveResourceFile(const std::string& filename, const std::string& fileContent);

    bool onRegularFileDownloaded(const ResponseContentResult& result, const std::string& fileName,
                                 const std::string& hash);
    bool onResourceFileDownloaded(const ResponseContentResult& result, const std::string& fileName);

    bool shouldBeDownloaded(const RegularFile& file) const;
    bool shouldBeDownloaded(const ResourceFile& resource) const;

    DownloadResult downloadRequiredFile(const ResourceFile& res);
    DownloadResult downloadRequiredFile(const RegularFile& file);
    DownloadResult downloadHttpFile(const RegularFile& file);
    DownloadResult downloadXmdsFile(const RegularFile& file);

private:
    XmdsRequestSender& m_xmdsRequestSender;
    std::unique_ptr<XmdsFileDownloader> m_xmdsFileDownloader;
};
