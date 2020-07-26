#pragma once

#include "common/crypto/Md5Hash.hpp"
#include "common/dt/DateTime.hpp"

#include <istream>
#include <string>
#include <vector>

class RegularFile
{
public:
    enum class DownloadType
    {
        HTTP,
        XMDS,
        Invalid
    };

    RegularFile(int id,
                size_t size,
                const Md5Hash& hash,
                const std::string& url,
                const std::string& name,
                const std::string& type,
                DownloadType downloadType);

    int id() const;
    size_t size() const;
    Md5Hash hash() const;
    std::string url() const;
    std::string name() const;
    std::string type() const;
    DownloadType downloadType() const;

private:
    friend std::ostream& operator<<(std::ostream& out, const RegularFile& file);

private:
    int id_;
    size_t size_;
    Md5Hash hash_;
    std::string url_;
    std::string name_;
    std::string type_;
    DownloadType downloadType_;
};

class ResourceFile
{
public:
    ResourceFile(int layoutId, int regionId, int mediaId, const DateTime& lastUpdate);

    int layoutId() const;
    int regionId() const;
    int mediaId() const;
    DateTime lastUpdate() const;
    std::string name() const;

private:
    friend std::ostream& operator<<(std::ostream& out, const ResourceFile& res);

private:
    int layoutId_;
    int regionId_;
    int mediaId_;
    DateTime lastUpdate_;
};

std::ostream& operator<<(std::ostream& out, const RegularFile& file);
std::ostream& operator<<(std::ostream& out, const ResourceFile& res);

template <typename RequriedFile>
using RequiredFilesSet = std::vector<RequriedFile>;
