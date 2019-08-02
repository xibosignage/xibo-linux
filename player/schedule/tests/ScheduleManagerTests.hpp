#pragma once

#include <gtest/gtest.h>

#include "common/fs/FileSystemAdaptor.hpp"
#include "common/fs/FileSystem.hpp"

#include "testutils/FakeFileSystemAdaptor.hpp"

class XiboLayoutScheduler;

class ScheduleManagerTests : public testing::Test
{
public:
    std::unique_ptr<XiboLayoutScheduler> construct()
    {
        return std::make_unique<XiboLayoutScheduler>();
    }

protected:
    static void TearDownTestCase()
    {
        FileSystem::setAdaptor(std::make_unique<FileSystemAdaptor>());
    }

    void SetUp() override
    {
        fs = new FakeFileSystemAdaptor;

        FileSystem::setAdaptor(std::unique_ptr<FakeFileSystemAdaptor>(fs));
    }

    void TearDown() override
    {
        fs = nullptr;
    }

    FakeFileSystemAdaptor* fs = nullptr;

};
