#pragma once

#include "control/media/MediaParser.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

class VideoParser : public MediaParser
{
public:
    VideoParser(const xml_node& node);
    MediaPlayerOptions parse();
};
