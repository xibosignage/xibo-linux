#include "RegisterDisplay.hpp"

#include "utils/Utilities.hpp"

constexpr const std::string_view REQUEST_NAME = "RegisterDisplay";

SOAP::RequestSerializer<RegisterDisplay::Request>::RequestSerializer(const RegisterDisplay::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<RegisterDisplay::Request>::string()
{
    return createRequest(REQUEST_NAME, request().clientCode, request().clientType, request().clientVersion,
                         request().displayName, request().macAddress, request().serverKey, request().hardwareKey);

}

SOAP::ResponseParser<RegisterDisplay::Response>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

RegisterDisplay::Response SOAP::ResponseParser<RegisterDisplay::Response>::get()
{
    auto activationMessage = responseTree().get_child("ActivationMessage").get_value<std::string>();
    auto display = Utils::parseXmlFromString(activationMessage).get_child("display");
    auto attrs = display.get_child("<xmlattr>");

    RegisterDisplay::Response result;
    result.status = static_cast<RegisterDisplay::Response::Status>(attrs.get<int>("status"));
    result.statusMessage = attrs.get<std::string>("message");
    if(result.status == RegisterDisplay::Response::Status::Ready)
    {
        fillPlayerSettings(result.playerSettings, display);
    }
    return result;
}

void SOAP::ResponseParser<RegisterDisplay::Response>::fillPlayerSettings(PlayerSettings& settings, const boost::property_tree::ptree& display)
{
    settings.collectInterval = display.get<int>("collectInterval");
    settings.downloadStartWindow = display.get<std::string>("downloadStartWindow");
    settings.downloadEndWindow = display.get<std::string>("downloadEndWindow");
    settings.statsEnabled = display.get<bool>("statsEnabled");
    settings.xmrNetworkAddress = display.get<std::string>("xmrNetworkAddress");
    settings.sizeX = display.get<double>("sizeX");
    settings.sizeY = display.get<double>("sizeY");
    settings.offsetX = display.get<double>("offsetX");
    settings.offsetY = display.get<double>("offsetY");
    settings.logLevel = display.get<std::string>("logLevel");
    settings.shellCommandsEnabled = display.get<bool>("enableShellCommands");
    settings.modifiedLayoutsEnabled = display.get<bool>("expireModifiedLayouts");
    settings.maxConcurrentDownloads = display.get<int>("maxConcurrentDownloads");
    //shellCommandAllowList
    settings.statusLayoutUpdate = display.get<bool>("sendCurrentLayoutAsStatusUpdate");
    settings.screenshotInterval = display.get<int>("screenShotRequestInterval");
    settings.screenshotSize = display.get<int>("screenShotSize");
    settings.maxLogFilesUploads = display.get<int>("maxLogFileUploads");
    settings.embeddedServerPort = display.get<int>("embeddedServerPort");
    settings.preventSleep = display.get<bool>("preventSleep");
    settings.displayName = display.get<std::string>("displayName");
    settings.screenshotRequested = display.get<bool>("screenShotRequested");
}
