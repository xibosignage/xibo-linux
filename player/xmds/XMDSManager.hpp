#ifndef XMDSMANAGER_H
#define XMDSMANAGER_H

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "Schedule.hpp"
#include "GetResource.hpp"

class SOAPManager;

using RegisterDisplayCallback = std::function<void(const RegisterDisplay::Response&)>;
using RequiredFilesCallback = std::function<void(const RequiredFiles::Response)>;
using ScheduleCallback = std::function<void(const Schedule::Response&)>;
using GetResourceCallback = std::function<void(const GetResource::Response&)>;

#include <future>

class XMDSManager
{
public:
    XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    std::future<RegisterDisplay::Response> registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName);
    std::future<RequiredFiles::Response> requiredFiles();
    std::future<Schedule::Response> schedule();
    std::future<GetResource::Response> getResource(int layoutId, int regionId, int mediaId);

private:
    std::unique_ptr<SOAPManager> m_soapManager;
    std::string m_serverKey;
    std::string m_hardwareKey;

};

#endif // XMDSMANAGER_H
