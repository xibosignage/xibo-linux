#pragma once

#include "control/media/MediaParser.hpp"
#include "MediaPlayerOptions.hpp"

class AudioParser : public MediaParser
{
public:
    AudioParser(const xml_node& node);
    MediaPlayerOptions parse();
    MediaPlayerOptions parseNode();

};
