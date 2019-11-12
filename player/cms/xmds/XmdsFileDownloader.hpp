#pragma once

#include <boost/thread/future.hpp>

#include "networking/ResponseResult.hpp"
#include "cms/xmds/GetFile.hpp"

using DownloadXmdsFilesResult = std::vector<boost::future<ResponseResult<GetFile::Result>>>;
using XmdsResponseResult = ResponseResult<std::string>;
class XmdsRequestSender;

class XmdsFileDownloader
{
public:
    XmdsFileDownloader(XmdsRequestSender& xmdsSender);
    boost::future<XmdsResponseResult> download(int fileId, const std::string& fileType, std::size_t fileSize);

private:
    XmdsResponseResult combineAllChunks(DownloadXmdsFilesResult& results);

private:
    XmdsRequestSender& xmdsSender_;
};
