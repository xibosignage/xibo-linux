#pragma once

#include <gtest/gtest.h>

#include "common/fs/FileSystem.hpp"
#include "common/fs/FileCache.hpp"

class Scheduler;

class SchedulerTests : public testing::Test
{
public:
    std::unique_ptr<Scheduler> construct()
    {
        return std::make_unique<Scheduler>(*m_fileCache);
    }

    void SetUp() override
    {
        m_fileCache = std::make_unique<FileCache>();
    }

    void TearDown() override
    {
        m_fileCache.reset();
    }

private:
    std::unique_ptr<FileCache> m_fileCache;

};
