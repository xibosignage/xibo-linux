#pragma once

#include "managers/RequiredItems.hpp"

class MediaInventoryItem
{
public:
    MediaInventoryItem(const RegularFile& file, bool downloadComplete_) :
        type(file.type), id(file.id), downloadComplete(downloadComplete_), md5(file.md5), lastChecked("today")
    {
    }

    MediaInventoryItem(const ResourceFile& file, bool downloadComplete_) :
        type("resource"), id(file.mediaId), downloadComplete(downloadComplete_), lastChecked("today")
    {
    }

    std::string type;
    int id;
    bool downloadComplete;
    std::string md5;
    std::string lastChecked;
};

using MediaInventoryItems = std::vector<MediaInventoryItem>;
