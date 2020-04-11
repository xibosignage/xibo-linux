#include "XmdsFileDownloader.hpp"

#include "cms/GetFileCommand.hpp"
#include "common/crypto/CryptoUtils.hpp"

const std::size_t DefaultChunkSize = 524288;

XmdsFileDownloader::XmdsFileDownloader(const CmsSettings& settings) : settings_(settings) {}

boost::future<XmdsResponseResult> XmdsFileDownloader::download(int fileId,
                                                               const std::string& fileType,
                                                               std::size_t fileSize)
{
    std::size_t fileOffset = 0;
    DownloadXmdsFilesResult results;

    while (fileOffset < fileSize)
    {
        std::size_t chunkSize = fileOffset + DefaultChunkSize >= fileSize ? fileSize - fileOffset : DefaultChunkSize;

        auto getFile = GetFileCommand::create(
            settings_.address(), settings_.key(), settings_.displayId(), fileId, fileType, fileOffset, chunkSize);
        results.emplace_back(getFile->executeFuture());

        fileOffset += chunkSize;
    }

    return boost::async([=, results = std::move(results)]() mutable { return combineAllChunks(results); });
}

XmdsResponseResult XmdsFileDownloader::combineAllChunks(DownloadXmdsFilesResult& results)
{
    std::string fileContent;
    for (auto&& future : results)
    {
        auto [error, result] = future.get();
        if (!error)
        {
            fileContent += CryptoUtils::fromBase64(result.base64chunk);
        }
        else
        {
            return {error, std::string{}};
        }
    }
    return {PlayerError{}, fileContent};
}
