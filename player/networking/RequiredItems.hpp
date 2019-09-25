#pragma once

#include <istream>
#include <string>
#include <vector>

struct RegularFile
{
    enum class DownloadType
    {
        HTTP,
        XMDS,
        Invalid
    };

    int id;
    size_t size;
    std::string hash;
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

std::ostream& operator<<(std::ostream& out, const RegularFile& file);
std::ostream& operator<<(std::ostream& out, const ResourceFile& res);

template <typename RequriedFile>
using FilesToDownload = std::vector<RequriedFile>;
