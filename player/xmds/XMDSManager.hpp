#ifndef XMDSMANAGER_H
#define XMDSMANAGER_H

#include <future>

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "Schedule.hpp"
#include "GetResource.hpp"
#include "GetFile.hpp"

class SOAPManager;

class XMDSManager
{
public:
    XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    std::future<RegisterDisplay::Result> registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName);
    std::future<RequiredFiles::Result> requiredFiles();
    std::future<Schedule::Result> schedule();
    std::future<GetResource::Result> getResource(int layoutId, int regionId, int mediaId);
    std::future<GetFile::Result> getFile(int fileId, const std::string& fileType, std::size_t chunkOffset, std::size_t chunkSize);

private:
    std::unique_ptr<SOAPManager> m_soapManager;
    std::string m_serverKey;
    std::string m_hardwareKey;

};

#endif // XMDSMANAGER_H
