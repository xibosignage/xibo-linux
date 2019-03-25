#pragma once

#include <boost/thread/future.hpp>

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "Schedule.hpp"
#include "GetResource.hpp"
#include "GetFile.hpp"
#include "MediaInventory.hpp"
#include "SubmitScreenShot.hpp"

#include "utils/ResponseResult.hpp"

class SOAPManager;

class XMDSManager
{
public:
    XMDSManager(std::string_view host, std::string_view serverKey, std::string_view hardwareKey);

    boost::future<ResponseResult<RegisterDisplay::Result>> registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName);
    boost::future<ResponseResult<RequiredFiles::Result>> requiredFiles();
    boost::future<ResponseResult<Schedule::Result>> schedule();
    boost::future<ResponseResult<GetResource::Result>> getResource(int layoutId, int regionId, int mediaId);
    boost::future<ResponseResult<GetFile::Result>> getFile(int fileId, const std::string& fileType, std::size_t chunkOffset, std::size_t chunkSize);
    boost::future<ResponseResult<MediaInventory::Result>> mediaInventory(MediaInventoryItems&& items);
    boost::future<ResponseResult<SubmitScreenShot::Result>> submitScreenShot(const std::string& screenShot);

private:
    std::unique_ptr<SOAPManager> m_soapManager;
    std::string m_serverKey;
    std::string m_hardwareKey;

};
