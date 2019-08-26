#pragma once

#include <gmock/gmock.h>

#include "common/fs/IFileCache.hpp"

class FakeFileCache : public IFileCache
{
public:
    MOCK_METHOD1(loadFrom, void(const FilePath& cacheFile));
    MOCK_CONST_METHOD2(cachedWithHash, bool(const std::string& file, const std::string& targetHash));
    MOCK_CONST_METHOD1(valid, bool(const std::string& file));
    MOCK_METHOD2(save, void(const std::string& file, const std::string& content));
};
