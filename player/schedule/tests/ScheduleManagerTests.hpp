#pragma once

#include <gtest/gtest.h>

#include "common/fs/FileSystemAdaptor.hpp"
#include "common/fs/FileSystem.hpp"

#include "mocks/FakeScheduleSerializer.hpp"
#include "testutils/FakeFileSystemAdaptor.hpp"

class ScheduleManager;

class ScheduleManagerTests : public testing::Test
{
public:
    std::unique_ptr<ScheduleManager> construct()
    {
        return std::make_unique<ScheduleManager>(std::unique_ptr<FakeScheduleSerializer>(serializer));
    }

protected:
    static void SetUpTestCase()
    {
        fs = new FakeFileSystemAdaptor;
        FileSystem::setAdaptor(std::unique_ptr<FakeFileSystemAdaptor>(fs));
    }

    static void TearDownTestCase()
    {
        fs = nullptr;
        FileSystem::setAdaptor(std::make_unique<FileSystemAdaptor>());
    }

    void SetUp() override
    {
        serializer = new FakeScheduleSerializer;
    }

    void TearDown() override
    {
        serializer = nullptr;
    }

    static inline FakeFileSystemAdaptor* fs = nullptr;
    FakeScheduleSerializer* serializer = nullptr;

};
