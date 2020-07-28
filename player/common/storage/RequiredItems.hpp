#pragma once

#include "common/crypto/Md5Hash.hpp"
#include "common/dt/DateTime.hpp"

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
    Md5Hash hash;
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
    DateTime lastUpdate;
};

std::ostream& operator<<(std::ostream& out, const RegularFile& file);
std::ostream& operator<<(std::ostream& out, const ResourceFile& res);

template <typename RequriedFile>
using FilesToDownload = std::vector<RequriedFile>;
