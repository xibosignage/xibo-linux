#include "RegisterDisplay.hpp"

#include "xmds.hpp"
#include "control/PlayerSettings.hpp"

template<>
std::string soap::requestString(const RegisterDisplay::Request& request)
{
    return createRequest<RegisterDisplay::Request>
            (request.clientCode, request.clientType, request.clientVersion,
             request.displayName, request.macAddress, request.serverKey, request.hardwareKey);
}

template<>
RegisterDisplay::Response soap::createResponse(const std::string& soapResponse)
{
    auto display = xmds::parseXmlResponse(soapResponse).get_child("display");
    auto attrs = display.get_child("<xmlattr>");

    RegisterDisplay::Response result;
    result.status = static_cast<RegisterDisplay::Response::Status>(attrs.get<int>("status"));
    result.statusMessage = attrs.get<std::string>("message");
    if(result.status == RegisterDisplay::Response::Status::Ready)
    {
        result.playerSettings.collectInterval = display.get<int>("collectInterval");
        result.playerSettings.downloadStartWindow = display.get<std::string>("downloadStartWindow");
        result.playerSettings.downloadEndWindow = display.get<std::string>("downloadEndWindow");
        result.playerSettings.statsEnabled = display.get<bool>("statsEnabled");
        result.playerSettings.xmrNetworkAddress = display.get<std::string>("xmrNetworkAddress");
        result.playerSettings.sizeX = display.get<double>("sizeX");
        result.playerSettings.sizeY = display.get<double>("sizeY");
        result.playerSettings.offsetX = display.get<double>("offsetX");
        result.playerSettings.offsetY = display.get<double>("offsetY");
        result.playerSettings.logLevel = display.get<std::string>("logLevel");
        result.playerSettings.shellCommandsEnabled = display.get<bool>("enableShellCommands");
        result.playerSettings.modifiedLayoutsEnabled = display.get<bool>("expireModifiedLayouts");
        result.playerSettings.maxConcurrentDownloads = display.get<int>("maxConcurrentDownloads");
        //shellCommandAllowList
        result.playerSettings.statusLayoutUpdate = display.get<bool>("sendCurrentLayoutAsStatusUpdate");
        result.playerSettings.screenshotInterval = display.get<int>("screenShotRequestInterval");
        result.playerSettings.screenshotSize = display.get<int>("screenShotSize");
        result.playerSettings.maxLogFilesUploads = display.get<int>("maxLogFileUploads");
        result.playerSettings.embeddedServerPort = display.get<int>("embeddedServerPort");
        result.playerSettings.preventSleep = display.get<bool>("preventSleep");
        result.playerSettings.displayName = display.get<std::string>("displayName");
        result.playerSettings.screenshotRequested = display.get<bool>("screenShotRequested");
    }
    return result;
}
