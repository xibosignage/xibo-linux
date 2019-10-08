#include <gtest/gtest.h>
#include <spdlog/sinks/null_sink.h>

#include "common/logger/Logging.hpp"

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::null_sink_st>()};
    Log::create(sinks);

    return RUN_ALL_TESTS();
}
