#pragma once

#include <string>
#include <vector>
#include <iostream>

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

inline std::ostream& operator<<(std::ostream& out, const RegularFile& file)
{
    out << "FileType: " << file.type << " ID: " << file.id << " Size: " << file.size << std::endl;
    out << "MD5: " << file.md5 << " FileName: " << file.name << " DownloadType: " << static_cast<int>(file.downloadType);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const ResourceFile& res)
{
    return out << "Layout ID: " << res.layoutId << " Region ID: " << res.regionId << " Media ID: " << res.mediaId;
}

template<typename RequriedFile>
using FilesToDownload = std::vector<RequriedFile>;
