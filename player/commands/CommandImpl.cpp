#include "CommandImpl.hpp"

#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/thread/future.hpp>
#include <sstream>

#include <iostream>

using namespace std::string_literals;
namespace bp = boost::process;

CommandImpl::CommandImpl(std::string commandString) : commandString_{std::move(commandString)} {}

CommandImpl::CommandImpl(std::string commandString, std::string validationString) :
    commandString_{std::move(commandString)}, validationString_{std::move(validationString)}
{
}

Command::ExecutionResult CommandImpl::execute()
{
    try
    {
        ios_ = std::make_unique<boost::asio::io_service>();
        boost::asio::streambuf output, error;
        process_ = std::make_unique<bp::child>(
            bp::shell(), bp::args({"-c", commandString_}), bp::std_out > output, bp::std_err > error, *ios_);

        ios_->run();

        std::error_code ec;
        process_->wait(ec);

        auto toString = [](std::streambuf& buf) { return std::string(std::istreambuf_iterator<char>(&buf), {}); };

        if (ec)
        {
            return ExecutionResult{Status::Error, ExecutionError{process_->exit_code(), ec.message()}};
        }
        else if (process_->exit_code() != SuccessExitCode)
        {
            return ExecutionResult{Status::Error, ExecutionError{process_->exit_code(), toString(error)}};
        }

        return validateExecutionResult(toString(output));
    }
    catch (const std::exception& e)
    {
        return ExecutionResult{Status::Error, ExecutionError{-1, "failed to execute command: "s + e.what()}};
    }
}

void CommandImpl::executeAsync(CommandExecuted&& callback)
{
    std::thread([command = shared_from_this(), callback = std::move(callback)]() {
        callback(command->execute());
    }).detach();
}

// TODO: exception here
void CommandImpl::terminate()
{
    if (process_ && process_->running())
    {
        process_->terminate();
    }
    if (ios_ && !ios_->stopped())
    {
        ios_->stop();
    }
}

const std::string& CommandImpl::executableString() const
{
    return commandString_;
}

const std::string& CommandImpl::validationString() const
{
    return validationString_;
}

Command::ExecutionResult CommandImpl::validateExecutionResult(const std::string& commandOutput) const
{
    const ExecutionResult SuccessExecutionResult{Status::Success, ExecutionError{}};
    if (!validationString_.empty())
    {
        return commandOutput == validationString_
                   ? SuccessExecutionResult
                   : ExecutionResult{
                         Status::Error,
                         ExecutionError{SuccessExitCode,
                                        "command output '"s + commandOutput + "' doesn't match validation string"s}};
    }
    return SuccessExecutionResult;
}