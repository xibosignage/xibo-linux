#pragma once

#include <gtest/gtest.h>

#include "FakeFileCache.hpp"
#include "Common.hpp"

class Scheduler;

class SchedulerTests : public testing::Test
{
public:
    std::unique_ptr<Scheduler> construct()
    {
        return std::make_unique<Scheduler>(*fileCache);
    }

    void SetUp() override
    {
        fileCache = std::make_unique<testing::NiceMock<FakeFileCache>>();
    }

    void TearDown() override
    {
        fileCache.reset();
    }

protected:
    DefaultScheduledLayout defaultLayout(int id)
    {
        DefaultScheduledLayout layout;

        ON_CALL(*this->fileCache, cached(std::to_string(DefaultId) + ".xlf")).WillByDefault(testing::Return(true));
        layout.id = id;

        return layout;
    }

protected:
    std::unique_ptr<FakeFileCache> fileCache;

};
