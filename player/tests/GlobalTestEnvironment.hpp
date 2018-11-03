#pragma once

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/null_sink.h>

#include "constants.hpp"
#include "utils/Resources.hpp"

const std::string DEFAULT_RESOURCES_DIR = "fakedir";

class GlobalTestEnvironment : public ::testing::Environment
{
public:
    void SetUp() override
    {
        Resources::setDirectory(DEFAULT_RESOURCES_DIR);
        m_logger = spdlog::create<spdlog::sinks::null_sink_st>(LOGGER);
    }

private:
    std::shared_ptr<spdlog::logger> m_logger;

};
