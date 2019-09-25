#include "MediaInventoryItem.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>

#include "common/logger/Logging.hpp"

const std::string DEFAULT_RESOURCE_TYPE = "resource";

MediaInventoryItem::MediaInventoryItem(const RegularFile& file, bool downloadComplete) :
    MediaInventoryItem(downloadComplete)
{
    m_type = file.type;
    m_id = file.id;
    m_md5 = file.hash;
}

MediaInventoryItem::MediaInventoryItem(const ResourceFile& file, bool downloadComplete) :
    MediaInventoryItem(downloadComplete)
{
    m_type = DEFAULT_RESOURCE_TYPE;
    m_id = file.mediaId;
}

MediaInventoryItem::MediaInventoryItem(bool downloadComplete) : m_downloadComplete(downloadComplete)
{
    auto timepoint = std::chrono::system_clock::now();
    auto duration = timepoint.time_since_epoch();
    m_lastChecked = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
}

const std::string& MediaInventoryItem::type() const
{
    return m_type;
}

int MediaInventoryItem::id() const
{
    return m_id;
}

bool MediaInventoryItem::downloadComplete() const
{
    return m_downloadComplete;
}

const std::string& MediaInventoryItem::md5() const
{
    return m_md5;
}

const std::string& MediaInventoryItem::lastChecked() const
{
    return m_lastChecked;
}
