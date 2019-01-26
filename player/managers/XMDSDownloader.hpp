#pragma once

#include <boost/thread/future.hpp>

#include "xmds/GetFile.hpp"
#include "utils/ResponseResult.hpp"

using DownloadXMDSFilesResult = std::vector<boost::future<ResponseResult<GetFile::Result>>>;
using XMDSResponseResult = ResponseResult<std::string>;

class XMDSDownloader
{
public:
    boost::future<XMDSResponseResult> download(int fileId, const std::string& fileType, std::size_t fileSize);

private:
    XMDSResponseResult combineAllChunks(DownloadXMDSFilesResult& results);

};
