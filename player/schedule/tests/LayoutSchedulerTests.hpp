#pragma once

#include <gtest/gtest.h>

#include "common/fs/FileSystemAdaptor.hpp"
#include "common/fs/FileSystem.hpp"

#include "testutils/FakeFileSystemAdaptor.hpp"

class LayoutScheduler;

class LayoutSchedulerTests : public testing::Test
{
public:
    std::unique_ptr<LayoutScheduler> construct()
    {
        return std::make_unique<LayoutScheduler>();
    }
};
