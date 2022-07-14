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

// TODO: investigate lock-free solution
Command::ExecutionResult CommandImpl::execute()
{
    try
    {
        std::unique_lock lock{processLock_};

        if (!isTerminatedInternal()) throw Error{"command is already being executed"};

        boost::asio::streambuf output, error;
        ios_ = std::make_unique<boost::asio::io_service>();
        process_ = std::make_unique<bp::child>(
            bp::shell(), bp::args({"-c", commandString_}), bp::std_out > output, bp::std_err > error, *ios_);

        running_ = true; // FIXME: false on destruction
        lock.unlock();

        ios_->run();

        lock.lock();
        running_ = false; 

        // FIXME: probably use boost::group if the launched process also has children
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
    catch (const PlayerRuntimeError& e)
    {
        return ExecutionResult{Status::Error, ExecutionError{-1, "failed to execute command: "s + e.message()}};
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

bool CommandImpl::isTerminated() const
{
    std::unique_lock lock{processLock_};

    return isTerminatedInternal();
}

bool CommandImpl::isTerminatedInternal() const
{
    return !running_;
}

void CommandImpl::terminate()
{
    std::unique_lock lock{processLock_};

    if (isTerminatedInternal()) return;

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