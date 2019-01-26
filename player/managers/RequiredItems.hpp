#pragma once

#include <string>
#include <vector>

enum class DownloadType
{
    HTTP,
    XMDS,
    Invalid
};

struct RegularFile
{
    int id;
    size_t size;
    std::string md5;
    std::string url;
    std::string name;
    std::string type;
    DownloadType downloadType;
};

struct ResourceFile
{
    int layoutId;
    int regionId;
    int mediaId;
};

using RegularFiles = std::vector<RegularFile>;
using ResourceFiles = std::vector<ResourceFile>;

