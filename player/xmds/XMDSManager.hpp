#ifndef XMDSMANAGER_H
#define XMDSMANAGER_H

#include <future>

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "Schedule.hpp"
#include "GetResource.hpp"

class SOAPManager;

using RegisterDisplayCallback = std::function<void(const RegisterDisplay::Result&)>;
using RequiredFilesCallback = std::function<void(const RequiredFiles::Result)>;
using ScheduleCallback = std::function<void(const Schedule::Result&)>;
using GetResourceCallback = std::function<void(const GetResource::Result&)>;

class XMDSManager
{
public:
    XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    std::future<RegisterDisplay::Result> registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName);
    std::future<RequiredFiles::Result> requiredFiles();
    std::future<Schedule::Result> schedule();
    std::future<GetResource::Result> getResource(int layoutId, int regionId, int mediaId);

private:
    std::unique_ptr<SOAPManager> m_soapManager;
    std::string m_serverKey;
    std::string m_hardwareKey;

};

#endif // XMDSMANAGER_H
