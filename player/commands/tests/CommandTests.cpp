#include <gtest/gtest.h>

#include "commands/CommandImpl.hpp"
#include "common/JoinableThread.hpp"

#include <boost/process/system.hpp>
#include <optional>

using namespace std::chrono_literals;

struct SyncTag
{
};

struct AsyncTag
{
};

template <typename T>
class CommandTests : public testing::TestWithParam<T>
{
    using OptionalResult = std::optional<Command::ExecutionResult>;

public:
    Command::ExecutionResult execute(Command& command)
    {
        if constexpr (std::is_same_v<T, SyncTag>)
        {
            return command.execute();
        }
        else
        {
            auto optionalResult = std::make_shared<OptionalResult>();

            command.executeAsync(
                [this, optionalResultWeak = std::weak_ptr{optionalResult}](Command::ExecutionResult result) {
                    // FIXME: race condition here
                    if (auto optionalResult = optionalResultWeak.lock())
                    {
                        *optionalResult = std::move(result);
                    }
                });

            std::this_thread::sleep_for(250ms);

            if (!optionalResult->has_value()) throw std::runtime_error{"Result not ready"};
            return optionalResult->value();
        }
    }
};

using ExecutionTags = ::testing::Types<SyncTag, AsyncTag>;
TYPED_TEST_CASE(CommandTests, ExecutionTags);

std::shared_ptr<Command> createCommand(std::string commandString, std::string validationString = {})
{
    return std::make_unique<CommandImpl>(std::move(commandString), std::move(validationString));
}

TYPED_TEST(CommandTests, SuccessExecutionSimpleCommand)
{
    auto command = createCommand("echo hello");

    auto [status, error] = this->execute(*command);

    ASSERT_EQ(status, Command::Status::Success);
    ASSERT_TRUE(command->isTerminated());
}

TYPED_TEST(CommandTests, SuccessExecutionSimpleCommandValidation)
{
    auto command = createCommand("echo hello", "hello\n");

    auto [status, error] = this->execute(*command);

    ASSERT_EQ(status, Command::Status::Success);
    ASSERT_TRUE(command->isTerminated());
}

TYPED_TEST(CommandTests, SuccessExecutionCommandWithArgs)
{
    auto command = createCommand("echo -n hello");

    auto [status, error] = this->execute(*command);

    ASSERT_EQ(status, Command::Status::Success);
    ASSERT_TRUE(command->isTerminated());
}

TYPED_TEST(CommandTests, SuccessExecutionCommandWithArgsValidation)
{
    auto command = createCommand("echo -n hello", "hello");

    auto [status, error] = this->execute(*command);

    ASSERT_EQ(status, Command::Status::Success);
    ASSERT_TRUE(command->isTerminated());
}

TYPED_TEST(CommandTests, FailedExecutionUnknownCommand)
{
    auto command = createCommand("qweasd");

    auto [status, error] = this->execute(*command);

    ASSERT_EQ(status, Command::Status::Error);
    ASSERT_TRUE(command->isTerminated());
}

TYPED_TEST(CommandTests, FailedExecutionPermissionError)
{
    auto command = createCommand("rm /bin/sh");

    auto [status, error] = this->execute(*command);

    ASSERT_EQ(status, Command::Status::Error);
    ASSERT_TRUE(command->isTerminated());
}

TYPED_TEST(CommandTests, ProcessTerminationLoopedCommand)
{
    auto command = createCommand("yes");
    JoinableThread delayedTermination([&command]() {
        std::this_thread::sleep_for(100ms);
        command->terminate();
    });

    auto [status, error] = this->execute(*command);

    // NOTE: should it be treated as error?
    ASSERT_EQ(status, Command::Status::Error);
    ASSERT_TRUE(command->isTerminated());
}

TYPED_TEST(CommandTests, ProcessSigkillLoopedCommand)
{
    auto command = createCommand("yes");
    JoinableThread delayedTermination([&command]() {
        std::this_thread::sleep_for(100ms);
        boost::process::system("pkill yes");
    });

    auto [status, error] = this->execute(*command);

    ASSERT_EQ(status, Command::Status::Error);
    ASSERT_TRUE(command->isTerminated());
}

// TODO: potential tests for calling execute/executeAsync multiple times
