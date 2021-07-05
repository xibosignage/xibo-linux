#pragma once

#include "Command.hpp"

#include "common/PlayerRuntimeError.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/process/child.hpp>
#include <mutex>

class CommandImpl : public Command, public std::enable_shared_from_this<CommandImpl>
{
public:
    DECLARE_EXCEPTION(Command)

    template <typename... Args>
    static CommandPtr create(Args&&... args)
    {
        return std::make_shared<CommandImpl>(std::forward<Args>(args)...);
    }

    CommandImpl(std::string commandString);
    CommandImpl(std::string commandString, std::string validationString);

    ExecutionResult execute() override;
    void executeAsync(CommandExecuted&& callback) override;
    bool isTerminated() const override;
    void terminate() override;
    const std::string& executableString() const override;
    const std::string& validationString() const override;

private:
    bool isTerminatedInternal() const;
    ExecutionResult validateExecutionResult(const std::string& commandOutput) const;

private:
    mutable std::mutex processLock_;
    bool running_ = false;
    std::unique_ptr<boost::process::child> process_;
    std::unique_ptr<boost::asio::io_service> ios_;
    std::string commandString_;
    std::string validationString_;
};
