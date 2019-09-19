#include "RequiredItems.hpp"
#include "common/Utils.hpp"

template<>
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

std::ostream& operator<< (std::ostream& out, const RegularFile& file)
{
    out << "FileType: " << file.type << " ID: " << file.id << " Size: " << file.size << std::endl;
    out << "MD5: " << file.hash << " FileName: " << file.name << " DownloadType: " << Utils::toString(file.downloadType);
    return out;
}

std::ostream& operator<< (std::ostream& out, const ResourceFile& res)
{
    return out << "Layout ID: " << res.layoutId << " Region ID: " << res.regionId << " Media ID: " << res.mediaId;
}
