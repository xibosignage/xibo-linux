#pragma once

#include "XmdsCommand.hpp"
#include "cms/xmds/RegisterDisplay.hpp"

class RegisterDisplayCommand : public XmdsCommand<RegisterDisplay>
{
    using SignalSettingsUpdated = boost::signals2::signal<void(PlayerSettings)>;
    using SignalDisplayReady = boost::signals2::signal<void()>;

    static const inline std::string DefaultClientType = "linux";

public:
    static std::unique_ptr<RegisterDisplayCommand> create(const std::string& host,
                                                          const std::string& serverKey,
                                                          const std::string& hardwareKey,
                                                          const std::string& version,
                                                          const std::string& codeVersion,
                                                          const std::string& displayName);

    SignalSettingsUpdated& settingsUpdated();
    SignalDisplayReady& displayReady();

private:
    RegisterDisplayCommand(const std::string& host,
                           const std::string& serverKey,
                           const std::string& hardwareKey,
                           const std::string& version,
                           const std::string& codeVersion,
                           const std::string& displayName);

    void prepare(RegisterDisplay::Request& request) override;
    void process(const RegisterDisplay::Response& response) override;

    PlayerError displayStatus(const RegisterDisplay::Response::Status& status);

private:
    std::string xmrChannel_;
    std::string version_;
    std::string codeVersion_;
    std::string displayName_;
    SignalSettingsUpdated settingsUpdated_;
    SignalDisplayReady displayReady_;
};
