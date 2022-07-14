#pragma once

#include "CommandCode.hpp"
#include "Command.hpp"
#include "PredefinedCommands.hpp"

#include <mutex>
#include <set>

class CommandsExecutor
{
public:
    CommandsExecutor() = default;
    ~CommandsExecutor();

    void updatePredefinedCommands(PredefinedCommands&& map);

    void executeString(const std::string& commandString);
    void executeStringAsync(const std::string& commandString);

    void executePredefined(const CommandCode& code);
    void executePredefinedAsync(const CommandCode& code);

    void terminateAllCommands();

private:
    using ExecutionMethod = std::function<void(CommandDefinition)>;

    void executePredefinedImpl(const CommandCode& code, ExecutionMethod executionMethod);
    void execute(CommandDefinition definition);
    void executeAsync(CommandDefinition definition);

    bool commandExists(const CommandCode& code) const;
    CommandDefinition commandDefinition(const CommandCode& code) const;

    void addToExecutionSet(const CommandPtr& command);
    void removeFromExecutionSet(const CommandPtr& command);

    void processExecutionResult(const CommandPtr& command, const Command::ExecutionResult& executionResult);

private:
    std::mutex predefinedCommandsLock_;
    std::mutex executionSetLock_;
    std::set<CommandPtr> commandsInExecution_;
    PredefinedCommands predefinedCommands_;
};