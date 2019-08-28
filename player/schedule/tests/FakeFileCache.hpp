#pragma once

#include <gmock/gmock.h>

#include "common/fs/IFileCache.hpp"

class FakeFileCache : public IFileCache
{
public:
    MOCK_METHOD1(loadFrom, void(const FilePath& cacheFile));
    MOCK_CONST_METHOD1(valid, bool(const std::string& filename));
    MOCK_CONST_METHOD2(cached, bool(const std::string& filename, const std::string& hash));
    MOCK_METHOD1(markAsInvalid, void(const std::string& filename));
    MOCK_METHOD3(save, void(const std::string& filename, const std::string& content, const std::string& md5));
};
