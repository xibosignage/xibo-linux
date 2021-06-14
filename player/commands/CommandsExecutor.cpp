#include "CommandsExecutor.hpp"

#include "CommandImpl.hpp"

#include "common/logger/Logging.hpp"

void CommandsExecutor::updatePredefinedCommands(PredefinedCommands&& commands)
{
    // FIXME: update only changed commands
    // FIXME: what to do with commands in the process of execution
    predefinedCommands_ = std::move(commands);
}

void CommandsExecutor::executeString(const std::string& commandString)
{
    auto command = std::make_shared<CommandImpl>(commandString);
    processExecutionResult(commandString, command->execute());
}

void CommandsExecutor::executeStringAsync(const std::string& commandString)
{
    auto command = std::make_shared<CommandImpl>(commandString);
    command->executeAsync([this, commandString](const Command::ExecutionResult& result) {
        processExecutionResult(commandString, result);
    });
}

void CommandsExecutor::execute(const CommandCode& code)
{
    if (commandExists(code))
    {
        auto& command = *predefinedCommands_.at(code);
        processExecutionResult(command.executableString(), command.execute());
    }
    else
    {
        Log::error("[CommandsExecutor] Failed to execute non-existing command with code: {}",
                   static_cast<std::string>(code));
    }
}

void CommandsExecutor::executeAsync(const CommandCode& code)
{
    if (commandExists(code))
    {
        auto& command = *predefinedCommands_.at(code);
        command.executeAsync(
            [this, commandString = command.executableString()](const Command::ExecutionResult& result) {
                processExecutionResult(commandString, result);
            });
    }
    else
    {
        Log::error("[CommandsExecutor] Failed to execute non-existing command with code: {}",
                   static_cast<std::string>(code));
    }
}

bool CommandsExecutor::commandExists(const CommandCode& code) const
{
    return predefinedCommands_.count(code) > 0;
}

void CommandsExecutor::processExecutionResult(const std::string& commandString,
                                              const Command::ExecutionResult& executionResult)
{
    auto [status, error] = executionResult;
    if (status == Command::Status::Error)
    {
        Log::error("[CommandsExecutor] Command '{}' finished unsuccessfully (exit code: {}). Reason: {}",
                   commandString,
                   error.exitCode,
                   error.output);
    }
    else
    {
        Log::debug("[CommandsExecutor] Command '{}' finished successfully", commandString);
    }
}