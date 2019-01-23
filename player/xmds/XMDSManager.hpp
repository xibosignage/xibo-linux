#ifndef XMDSMANAGER_H
#define XMDSMANAGER_H

#include <boost/thread/future.hpp>

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "Schedule.hpp"
#include "GetResource.hpp"
#include "GetFile.hpp"
#include "utils/ResponseResult.hpp"

class SOAPManager;

class XMDSManager
{
public:
    XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    boost::future<ResponseResult<RegisterDisplay::Result>> registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName);
    boost::future<ResponseResult<RequiredFiles::Result>> requiredFiles();
    boost::future<ResponseResult<Schedule::Result>> schedule();
    boost::future<ResponseResult<GetResource::Result>> getResource(int layoutId, int regionId, int mediaId);
    boost::future<ResponseResult<GetFile::Result>> getFile(int fileId, const std::string& fileType, std::size_t chunkOffset, std::size_t chunkSize);

private:
    std::unique_ptr<SOAPManager> m_soapManager;
    std::string m_serverKey;
    std::string m_hardwareKey;

};

#endif // XMDSMANAGER_H
