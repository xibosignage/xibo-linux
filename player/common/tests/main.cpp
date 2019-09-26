#include <gtest/gtest.h>

#include "common/logger/Logger.hpp"
#include "constants.hpp"

#include <spdlog/sinks/null_sink.h>

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::null_sink_st>()};

    auto logger = XiboLogger::create(SpdLogger, sinks);
    logger->setLevel(LoggingLevel::Debug);
    logger->setPattern("[%H:%M:%S.%e] [%t] [%l]: %v");

    return RUN_ALL_TESTS();
}
