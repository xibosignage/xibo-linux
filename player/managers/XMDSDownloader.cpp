#include "XMDSDownloader.hpp"

#include "xmds/XMDSManager.hpp"
#include "utils/Utilities.hpp"

#include <boost/beast/core/detail/base64.hpp>

const std::size_t DEFAULT_CHUNK_SIZE = 524288;

boost::future<XMDSResponseResult> XMDSDownloader::download(int fileId, const std::string& fileType, std::size_t fileSize)
{
    std::size_t fileOffset = 0;
    DownloadXMDSFilesResult results;

    while(fileOffset < fileSize)
    {
        std::size_t chunkSize = fileOffset + DEFAULT_CHUNK_SIZE >= fileSize ? fileSize - fileOffset : DEFAULT_CHUNK_SIZE;

        results.emplace_back(Utils::xmdsManager().getFile(fileId, fileType, fileOffset, chunkSize));

        fileOffset += chunkSize;
    }

    return boost::async([=, results = std::move(results)]() mutable {
        return combineAllChunks(results);
    });
}

XMDSResponseResult XMDSDownloader::combineAllChunks(DownloadXMDSFilesResult& results)
{
    std::string fileContent;
    for(auto&& future : results)
    {
        auto [error, result] = future.get();
        if(!error)
        {
            fileContent += boost::beast::detail::base64_decode(result.base64chunk);
        }
        else
        {
            return {error, std::string{}};
        }
    }
    return {PlayerError{}, fileContent};
}
