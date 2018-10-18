#pragma once

#include "utils/IFileSystemAdaptor.hpp"
#include <gmock/gmock.h>

class MockFileSystemAdaptor : public IFileSystemAdaptor
{
public:
    MOCK_METHOD1(setResourcesDirectory, void(FilePath path));
    MOCK_CONST_METHOD0(resourcesDirectory, FilePath());
    MOCK_CONST_METHOD1(isRegularFile, bool(FilePath path));

};
