#pragma once

#include "CommandCode.hpp"
#include "Command.hpp"
#include "PredefinedCommands.hpp"

class CommandsExecutor
{
public:
    void updatePredefinedCommands(PredefinedCommands&& map);

    void executeString(const std::string& commandString);
    void executeStringAsync(const std::string& commandString);

    void execute(const CommandCode& code);
    void executeAsync(const CommandCode& code);

private:
    bool commandExists(const CommandCode& code) const;
    void processExecutionResult(const std::string& commandString, const Command::ExecutionResult& executionResult);

private:
    PredefinedCommands predefinedCommands_;
};