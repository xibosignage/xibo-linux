#pragma once

#include "managers/RequiredItems.hpp"

class MediaInventoryItem
{
public:
    MediaInventoryItem(const RegularFile& file, bool downloadComplete);
    MediaInventoryItem(const ResourceFile& file, bool downloadComplete);

    const std::string& type() const;
    int id() const;
    bool downloadComplete() const;
    const std::string& md5() const;
    const std::string& lastChecked() const;

private:
    MediaInventoryItem(bool downloadComplete);

    std::string m_type;
    int m_id;
    bool m_downloadComplete;
    std::string m_md5;
    std::string m_lastChecked;
};

using MediaInventoryItems = std::vector<MediaInventoryItem>;
