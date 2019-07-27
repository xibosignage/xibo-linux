#include <gmock/gmock.h>

#include "common/fs/IFileSystemAdaptor.hpp"
#include "common/fs/FilePath.hpp"

class FakeFileSystemAdaptor : public IFileSystemAdaptor
{
public:
    MOCK_METHOD1(isRegularFile, bool(const FilePath& path));
    MOCK_METHOD1(exists, bool(const FilePath& path));
    MOCK_METHOD1(createDirectory, bool(const FilePath& path));
    MOCK_METHOD2(writeToFile, void(const FilePath& path, const std::string& content));
    MOCK_CONST_METHOD0(currentPath, FilePath());
};
