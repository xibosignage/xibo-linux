#pragma once

#include "control/media/MediaParser.hpp"

#include "MediaPlayerOptions.hpp"

class VideoParser : public MediaParser
{
public:
    VideoParser(const xml_node& node);
    MediaPlayerOptions parse();
};
