#include "RegisterDisplayCommand.hpp"

#include "common/crypto/CryptoUtils.hpp"
#include "common/crypto/RsaManager.hpp"
#include "common/system/System.hpp"
#include "xmr/XmrChannel.hpp"

RegisterDisplayCommand::RegisterDisplayCommand(const std::string& host,
                                               const std::string& serverKey,
                                               const std::string& hardwareKey,
                                               const std::string& version,
                                               const std::string& codeVersion,
                                               const std::string& displayName) :
    XmdsCommand(host),
    host_(host),
    serverKey_(serverKey),
    hardwareKey_(hardwareKey),
    version_(version),
    codeVersion_(codeVersion),
    displayName_(displayName)
{
}

std::unique_ptr<RegisterDisplayCommand> RegisterDisplayCommand::create(const std::string& host,
                                                                       const std::string& serverKey,
                                                                       const std::string& hardwareKey,
                                                                       const std::string& version,
                                                                       const std::string& codeVersion,
                                                                       const std::string& displayName)
{
    return std::unique_ptr<RegisterDisplayCommand>(
        new RegisterDisplayCommand(host, serverKey, hardwareKey, version, codeVersion, displayName));
}

RegisterDisplayCommand::SignalSettingsUpdated& RegisterDisplayCommand::settingsUpdated()
{
    return settingsUpdated_;
}

RegisterDisplay::Request RegisterDisplayCommand::prepareRequest()
{
    RegisterDisplay::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.clientType = DefaultClientType;
    request.clientCode = codeVersion_;
    request.clientVersion = version_;
    request.macAddress = static_cast<std::string>(System::macAddress());
    request.xmrChannel = static_cast<std::string>(XmrChannel::fromCmsSettings(host_, serverKey_, hardwareKey_));
    request.xmrPubKey = CryptoUtils::keyToString(RsaManager::instance().publicKey());
    request.displayName = displayName_;
    return request;
}

void RegisterDisplayCommand::processResponse(const RegisterDisplay::Response& response)
{
    auto status = displayStatus(response.status);
    if (!status)
    {
        settingsUpdated_(response.playerSettings);
        finished()();
    }
    else
    {
        error()(status);
    }
}

PlayerError RegisterDisplayCommand::displayStatus(const RegisterDisplay::Response::Status& status)
{
    using DisplayCode = RegisterDisplay::Response::Status::Code;

    switch (status.code)
    {
        case DisplayCode::Ready: return {};
        case DisplayCode::Added:
        case DisplayCode::Waiting: return {"CMS", status.message};
        default: return {"CMS", "Unknown error with RegisterDisplay"};
    }
}
