#pragma once

#include <boost/thread/future.hpp>

#include "GetFile.hpp"
#include "GetResource.hpp"
#include "MediaInventory.hpp"
#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "Schedule.hpp"
#include "SubmitLog.hpp"
#include "SubmitScreenShot.hpp"

#include "common/uri/Uri.hpp"
#include "networking/ResponseResult.hpp"

class XmdsRequestSender
{
public:
    XmdsRequestSender(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    boost::future<ResponseResult<RegisterDisplay::Result>>
    registerDisplay(const std::string& clientCode, const std::string& clientVersion, const std::string& displayName);
    boost::future<ResponseResult<RequiredFiles::Result>> requiredFiles();
    boost::future<ResponseResult<Schedule::Result>> schedule();
    boost::future<ResponseResult<GetResource::Result>> getResource(int layoutId, int regionId, int mediaId);
    boost::future<ResponseResult<GetFile::Result>> getFile(int fileId, const std::string& fileType,
                                                           std::size_t chunkOffset, std::size_t chunkSize);
    boost::future<ResponseResult<MediaInventory::Result>> mediaInventory(MediaInventoryItems&& items);
    boost::future<ResponseResult<SubmitLog::Result>> submitLogs(const std::string& logXml);
    boost::future<ResponseResult<SubmitScreenShot::Result>> submitScreenShot(const std::string& screenShot);

private:
    Uri m_uri;
    std::string m_serverKey;
    std::string m_hardwareKey;
};
