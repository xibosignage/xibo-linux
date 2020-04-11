#pragma once

#include <boost/thread/future.hpp>

#include "cms/xmds/GetFile.hpp"
#include "config/CmsSettings.hpp"
#include "networking/ResponseResult.hpp"

using DownloadXmdsFilesResult = std::vector<boost::future<ResponseResult<GetFile::Response>>>;
using XmdsResponseResult = ResponseResult<std::string>;

class XmdsFileDownloader
{
public:
    XmdsFileDownloader(const CmsSettings& settings);

    boost::future<XmdsResponseResult> download(int fileId, const std::string& fileType, std::size_t fileSize);

private:
    XmdsResponseResult combineAllChunks(DownloadXmdsFilesResult& results);

private:
    CmsSettings settings_;
};
