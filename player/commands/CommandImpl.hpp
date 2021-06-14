#pragma once

#include "Command.hpp"

#include "common/PlayerRuntimeError.hpp"

#include <boost/process/child.hpp>
#include <boost/asio/io_service.hpp>

class CommandImpl : public Command, public std::enable_shared_from_this<CommandImpl>
{
public:
    DECLARE_EXCEPTION(Command)

    CommandImpl(std::string commandString);
    CommandImpl(std::string commandString, std::string validationString);

    ExecutionResult execute() override;
    void executeAsync(CommandExecuted&& callback) override;
    void terminate() override;
    const std::string& executableString() const override;
    const std::string& validationString() const override;

private:
    ExecutionResult validateExecutionResult(const std::string& commandOutput) const;

private:
    std::unique_ptr<boost::process::child> process_;
    std::unique_ptr<boost::asio::io_service> ios_;
    std::string commandString_;
    std::string validationString_;
};
