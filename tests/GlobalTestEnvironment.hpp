#pragma once

#include <gtest/gtest.h>
#include <spdlog/sinks/null_sink.h>

#include "constants.hpp"
#include "utils/logger/Logger.hpp"
#include "utils/Resources.hpp"

const std::string DEFAULT_RESOURCES_DIR = "fakedir";

class GlobalTestEnvironment : public ::testing::Environment
{
public:
    void SetUp() override
    {
        Logger::create(LOGGER, {std::make_shared<spdlog::sinks::null_sink_st>()});
        Resources::setDirectory(DEFAULT_RESOURCES_DIR);
    }

private:
    std::shared_ptr<spdlog::logger> m_logger;

};
