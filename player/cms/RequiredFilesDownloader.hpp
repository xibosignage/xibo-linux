#pragma once

#include "common/crypto/Md5Hash.hpp"
#include "common/logger/Logging.hpp"
#include "common/storage/RequiredItems.hpp"

#include "networking/ResponseResult.hpp"

#include <boost/thread/future.hpp>

using DownloadResult = boost::future<bool>;
using DownloadResults = std::vector<DownloadResult>;
using ResponseContentResult = ResponseResult<std::string>;

class XmdsFileDownloader;
class XmdsRequestSender;
class FileCache;

class RequiredFilesDownloader
{
public:
    RequiredFilesDownloader(XmdsRequestSender& xmdsRequestSender, FileCache& fileCache);
    ~RequiredFilesDownloader();

    template <typename RequiredFileType>
    boost::future<DownloadResults> download(const RequiredFilesSet<RequiredFileType>& files)
    {
        return boost::async(boost::launch::async, [this, files = std::move(files)]() {
            auto results = downloadAll(files);
            for (auto&& result : results)
            {
                result.wait();
            }
            return results;
        });
    }

private:
    template <typename RequiredFileType>
    DownloadResult tryDownloadRequiredFile(const RequiredFileType& requiredFile)
    {
        try
        {
            return downloadRequiredFile(requiredFile);
        }
        catch (std::exception& e)
        {
            Log::error("[RequiredFilesDownloader] {}", e.what());
        }
        return {};
    }

    template <typename RequiredFileType>
    DownloadResults downloadAll(const RequiredFilesSet<RequiredFileType>& requiredFiles)
    {
        DownloadResults results;

        for (auto&& file : requiredFiles)
        {
            Log::trace("[RequiredFilesDownloader] {}", file);

            if (shouldBeDownloaded(file))
            {
                auto result = tryDownloadRequiredFile(file);
                if (result.valid())
                {
                    results.emplace_back(std::move(result));
                }
            }
        }

        return results;
    }

    void saveRegularFile(const std::string& filename, const std::string& content, const Md5Hash& hash);
    void saveResourceFile(const std::string& filename, const std::string& fileContent);

    bool onRegularFileDownloaded(const ResponseContentResult& result, const std::string& fileName, const Md5Hash& hash);
    bool onResourceFileDownloaded(const ResponseContentResult& result, const std::string& fileName);

    bool shouldBeDownloaded(const RegularFile& file) const;
    bool shouldBeDownloaded(const ResourceFile& resource) const;

    DownloadResult downloadRequiredFile(const ResourceFile& res);
    DownloadResult downloadRequiredFile(const RegularFile& file);
    DownloadResult downloadHttpFile(const RegularFile& file);
    DownloadResult downloadXmdsFile(const RegularFile& file);

private:
    XmdsRequestSender& xmdsRequestSender_;
    FileCache& fileCache_;
    std::unique_ptr<XmdsFileDownloader> xmdsFileDownloader_;
};
