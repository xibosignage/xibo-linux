#include "XmdsFileDownloader.hpp"

#include "common/crypto/CryptoUtils.hpp"
#include "cms/xmds/XmdsRequestSender.hpp"

const std::size_t DefaultChunkSize = 524288;

XmdsFileDownloader::XmdsFileDownloader(XmdsRequestSender& xmdsSender) : xmdsSender_(xmdsSender) {}

boost::future<XmdsResponseResult> XmdsFileDownloader::download(int fileId,
                                                               const std::string& fileType,
                                                               std::size_t fileSize)
{
    std::size_t fileOffset = 0;
    DownloadXmdsFilesResult results;

    while (fileOffset < fileSize)
    {
        std::size_t chunkSize = fileOffset + DefaultChunkSize >= fileSize ? fileSize - fileOffset : DefaultChunkSize;

        results.emplace_back(xmdsSender_.getFile(fileId, fileType, fileOffset, chunkSize));

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
