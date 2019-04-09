#pragma once

#include "control/media/MediaParser.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

class AudioParser : public MediaParser
{
public:
    AudioParser(const xml_node& node);
    MediaPlayerOptions parse();
    MediaPlayerOptions parseMainNode();
    MediaPlayerOptions parseAdditionalNode();

private:
    bool m_isAdditionalNode = false;

};
