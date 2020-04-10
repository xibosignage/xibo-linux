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
    XmdsCommand(host, serverKey, hardwareKey),
    version_(version),
    codeVersion_(codeVersion),
    displayName_(displayName)
{
    xmrChannel_ = static_cast<std::string>(XmrChannel::fromCmsSettings(host, serverKey, hardwareKey));
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

RegisterDisplayCommand::SignalDisplayReady& RegisterDisplayCommand::displayReady()
{
    return displayReady_;
}

void RegisterDisplayCommand::prepare(RegisterDisplay::Request& request)
{
    request.clientType = DefaultClientType;
    request.clientCode = codeVersion_;
    request.clientVersion = version_;
    request.macAddress = static_cast<std::string>(System::macAddress());
    request.xmrChannel = xmrChannel_;
    request.xmrPubKey = CryptoUtils::keyToString(RsaManager::instance().publicKey());
    request.displayName = displayName_;
}

void RegisterDisplayCommand::process(const RegisterDisplay::Response& response)
{
    auto status = displayStatus(response.status);
    if (!status)
    {
        settingsUpdated_(response.playerSettings);
        displayReady_();
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
