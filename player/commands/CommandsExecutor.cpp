#include "CommandsExecutor.hpp"

#include "CommandImpl.hpp"

#include "common/logger/Logging.hpp"

CommandsExecutor::~CommandsExecutor()
{
    terminateAllCommands();
}

void CommandsExecutor::updatePredefinedCommands(PredefinedCommands&& commands)
{
    std::unique_lock lock{predefinedCommandsLock_};
    predefinedCommands_ = std::move(commands);
}

void CommandsExecutor::executeString(const std::string& commandString)
{
    Log::debug("[CommandsExecutor] Trying to execute random command '{}'", static_cast<std::string>(commandString));

    execute(CommandDefinition{commandString, {}});
}

void CommandsExecutor::executeStringAsync(const std::string& commandString)
{
    Log::debug("[CommandsExecutor] Trying to execute random command async '{}'",
               static_cast<std::string>(commandString));

    executeAsync(CommandDefinition{commandString, {}});
}

void CommandsExecutor::executePredefined(const CommandCode& code)
{
    Log::debug("[CommandsExecutor] Trying to execute predefined command '{}'", static_cast<std::string>(code));

    executePredefinedImpl(code, [this](CommandDefinition definition) { execute(std::move(definition)); });
}

void CommandsExecutor::executePredefinedAsync(const CommandCode& code)
{
    Log::debug("[CommandsExecutor] Trying to execute predefined command async '{}'", static_cast<std::string>(code));

    executePredefinedImpl(code, [this](CommandDefinition definition) { executeAsync(std::move(definition)); });
}

void CommandsExecutor::terminateAllCommands()
{
    for (auto&& command : commandsInExecution_)
    {
        try
        {
            command->terminate();
        }
        catch (const std::exception& e)
        {
            Log::error("Failed to terminate command '{}'", command->executableString());
        }
    }
}

void CommandsExecutor::executePredefinedImpl(const CommandCode& code, ExecutionMethod executionMethod)
{
    std::unique_lock lock{predefinedCommandsLock_};
    if (commandExists(code))
    {
        auto definition = commandDefinition(code);
        lock.unlock();

        executionMethod(std::move(definition));
    }
    else
    {
        Log::error("[CommandsExecutor] Failed to execute non-existing command with code: {}",
                   static_cast<std::string>(code));
    }
}

void CommandsExecutor::execute(CommandDefinition definition)
{
    auto command = CommandImpl::create(definition.executableString, definition.validationString);

    addToExecutionSet(command);

    processExecutionResult(command, command->execute());
}

void CommandsExecutor::executeAsync(CommandDefinition definition)
{
    auto command = CommandImpl::create(definition.executableString, definition.validationString);

    addToExecutionSet(command);

    command->executeAsync(
        [this, command](const Command::ExecutionResult& result) { processExecutionResult(command, result); });
}

bool CommandsExecutor::commandExists(const CommandCode& code) const
{
    return predefinedCommands_.count(code) > 0;
}

CommandDefinition CommandsExecutor::commandDefinition(const CommandCode& code) const
{
    return predefinedCommands_.at(code);
}

void CommandsExecutor::addToExecutionSet(const CommandPtr& command)
{
    std::unique_lock lock{executionSetLock_};
    commandsInExecution_.emplace(command);
}

void CommandsExecutor::removeFromExecutionSet(const CommandPtr& command)
{
    std::unique_lock lock{executionSetLock_};
    commandsInExecution_.erase(command);
}

void CommandsExecutor::processExecutionResult(const CommandPtr& command,
                                              const Command::ExecutionResult& executionResult)
{
    auto [status, error] = executionResult;
    if (status == Command::Status::Error)
    {
        Log::error("[CommandsExecutor] Command '{}' finished unsuccessfully (exit code: {}). Reason: {}",
                   command->executableString(),
                   error.exitCode,
                   error.output);
    }
    else
    {
        Log::debug("[CommandsExecutor] Command '{}' finished successfully", command->executableString());
    }
    removeFromExecutionSet(command);
}