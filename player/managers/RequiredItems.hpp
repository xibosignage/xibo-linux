#pragma once

#include <string>
#include <vector>

enum class FileType
{
    Media,
    Layout,
    Resource,
    Invalid
};

enum class DownloadType
{
    HTTP,
    XMDS,
    Invalid
};

struct RequiredFile
{
    int id;
    size_t size;
    std::string md5;
    std::string url;
    std::string name;
    std::string type;
    DownloadType downloadType;
};

struct RequiredResource
{
    int layoutId;
    int regionId;
    int mediaId;
};

using RegularFiles = std::vector<RequiredFile>;
using ResourceFiles = std::vector<RequiredResource>;

