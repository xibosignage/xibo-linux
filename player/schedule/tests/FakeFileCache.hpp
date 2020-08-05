#pragma once

#include <gmock/gmock.h>

#include "common/storage/FileCache.hpp"

class FakeFileCache : public FileCache
{
public:
    MOCK_METHOD1(loadFrom, void(const FilePath& cacheFile));
    MOCK_CONST_METHOD1(valid, bool(const std::string& filename));
    MOCK_CONST_METHOD1(cached, bool(const RegularFile& file));
    MOCK_CONST_METHOD1(cached, bool(const ResourceFile& file));
    MOCK_CONST_METHOD2(cached, bool(const std::string& filename, const Md5Hash& hash));
    MOCK_CONST_METHOD0(cachedFiles, std::vector<std::string>());
    MOCK_CONST_METHOD0(invalidFiles, std::vector<std::string>());
    MOCK_METHOD1(markAsInvalid, void(const std::string& filename));
    MOCK_METHOD3(save, void(const std::string& filename, const std::string& content, const Md5Hash& md5));
    MOCK_METHOD3(save, void(const std::string& filename, const std::string& content, const DateTime& lastUpdate));
};
