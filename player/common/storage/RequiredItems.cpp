#include "RequiredItems.hpp"

#include "common/Utils.hpp"

template <>
std::string Utils::toString(RegularFile::DownloadType val)
{
    switch (val)
    {
        case RegularFile::DownloadType::HTTP: return "HTTP";
        case RegularFile::DownloadType::XMDS: return "XMDS";
        case RegularFile::DownloadType::Invalid: return "Invalid";
    }

    return "unknown";
}

std::ostream& operator<<(std::ostream& out, const RegularFile& file)
{
    out << "FileType: " << file.type_ << " ID: " << file.id_ << " Size: " << file.size_ << std::endl;
    out << "MD5: " << file.hash_ << " FileName: " << file.name_
        << " DownloadType: " << Utils::toString(file.downloadType_) << " URL: " << file.url_;
    return out;
}

std::ostream& operator<<(std::ostream& out, const ResourceFile& res)
{
    return out << "Layout ID: " << res.layoutId_ << " Region ID: " << res.regionId_ << " Media ID: " << res.mediaId_;
}

size_t RegularFile::size() const
{
    return size_;
}

Md5Hash RegularFile::hash() const
{
    return hash_;
}

std::string RegularFile::url() const
{
    return url_;
}

std::string RegularFile::name() const
{
    return name_;
}

std::string RegularFile::type() const
{
    return type_;
}

RegularFile::DownloadType RegularFile::downloadType() const
{
    return downloadType_;
}

RegularFile::RegularFile(int id,
                         size_t size,
                         const Md5Hash& hash,
                         const std::string& url,
                         const std::string& name,
                         const std::string& type,
                         RegularFile::DownloadType downloadType) :
    id_{id},
    size_{size},
    hash_{hash},
    url_{url},
    name_{name},
    type_{type},
    downloadType_{downloadType}
{
}

int RegularFile::id() const
{
    return id_;
}

ResourceFile::ResourceFile(int layoutId, int regionId, int mediaId, const DateTime& lastUpdate) :
    layoutId_{layoutId},
    regionId_{regionId},
    mediaId_{mediaId},
    lastUpdate_{lastUpdate}
{
}

int ResourceFile::layoutId() const
{
    return layoutId_;
}

int ResourceFile::regionId() const
{
    return regionId_;
}

int ResourceFile::mediaId() const
{
    return mediaId_;
}

DateTime ResourceFile::lastUpdate() const
{
    return lastUpdate_;
}

std::string ResourceFile::name() const
{
    return std::to_string(mediaId_) + ".html";
}
