#pragma once

#include "utils/IFileSystem.hpp"
#include <gmock/gmock.h>

class MockFileSystemAdaptor : public IFileSystemAdaptor
{
public:
    MOCK_CONST_METHOD1(isRegularFile, bool(const FilePath& path));
    MOCK_CONST_METHOD1(exists, bool(const FilePath& path));
};
