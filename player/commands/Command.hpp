#pragma once

#include <cstdint>
#include <functional>
#include <memory>

class Command
{
public:
    enum class Status
    {
        Success,
        Error
    };

    static constexpr const int32_t SuccessExitCode = 0;

    struct ExecutionError
    {
        int32_t exitCode = SuccessExitCode;
        std::string output;
    };

    using ExecutionResult = std::pair<Status, ExecutionError>;
    using CommandExecuted = std::function<void(ExecutionResult)>;

public:
    virtual ~Command() = default;
    virtual ExecutionResult execute() = 0;
    virtual void executeAsync(CommandExecuted&& callback) = 0;
    virtual bool isTerminated() const = 0;
    virtual void terminate() = 0;
    virtual const std::string& executableString() const = 0;
    virtual const std::string& validationString() const = 0;
};

using CommandPtr = std::shared_ptr<Command>;