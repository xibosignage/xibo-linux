#pragma once

#include <boost/thread/future.hpp>

#include "networking/ResponseResult.hpp"
#include "networking/xmds/GetFile.hpp"
#include "networking/xmds/GetResource.hpp"
#include "networking/xmds/MediaInventory.hpp"
#include "networking/xmds/RegisterDisplay.hpp"
#include "networking/xmds/RequiredFiles.hpp"
#include "networking/xmds/Schedule.hpp"
#include "networking/xmds/SubmitLog.hpp"
#include "networking/xmds/SubmitScreenShot.hpp"
#include "networking/xmds/SubmitStats.hpp"

#include "common/types/Uri.hpp"

template <typename Result>
using FutureResponseResult = boost::future<ResponseResult<Result>>;

class XmdsRequestSender
{
public:
    XmdsRequestSender(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    FutureResponseResult<RegisterDisplay::Result> registerDisplay(const std::string& clientCode,
                                                                  const std::string& clientVersion,
                                                                  const std::string& displayName);
    FutureResponseResult<RequiredFiles::Result> requiredFiles();
    FutureResponseResult<Schedule::Result> schedule();
    FutureResponseResult<GetResource::Result> getResource(int layoutId, int regionId, int mediaId);
    FutureResponseResult<GetFile::Result> getFile(int fileId,
                                                  const std::string& fileType,
                                                  std::size_t chunkOffset,
                                                  std::size_t chunkSize);
    FutureResponseResult<MediaInventory::Result> mediaInventory(MediaInventoryItems&& items);
    FutureResponseResult<SubmitLog::Result> submitLogs(const std::string& logXml);
    FutureResponseResult<SubmitStats::Result> submitStats(const std::string& statXml);
    FutureResponseResult<SubmitScreenShot::Result> submitScreenShot(const std::string& screenShot);

private:
    Uri uri_;
    std::string serverKey_;
    std::string hardwareKey_;
};
