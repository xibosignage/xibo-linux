#include "MediaInventoryItem.hpp"

#include "common/logger/Logging.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>

const std::string ResourceType = "resource";

MediaInventoryItem::MediaInventoryItem(const RegularFile& file, bool downloadComplete) :
    MediaInventoryItem(downloadComplete)
{
    type_ = file.type();
    id_ = file.id();
    md5_ = file.hash();
}

MediaInventoryItem::MediaInventoryItem(const ResourceFile& file, bool downloadComplete) :
    MediaInventoryItem(downloadComplete)
{
    type_ = ResourceType;
    id_ = file.mediaId();
}

MediaInventoryItem::MediaInventoryItem(bool downloadComplete) : downloadComplete_(downloadComplete)
{
    auto timepoint = std::chrono::system_clock::now();
    auto duration = timepoint.time_since_epoch();
    lastChecked_ = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
}

const std::string& MediaInventoryItem::type() const
{
    return type_;
}

int MediaInventoryItem::id() const
{
    return id_;
}

bool MediaInventoryItem::downloadComplete() const
{
    return downloadComplete_;
}

const Md5Hash& MediaInventoryItem::md5() const
{
    return md5_;
}

const std::string& MediaInventoryItem::lastChecked() const
{
    return lastChecked_;
}
