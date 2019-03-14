#pragma once

#include "MediaParser.hpp"

class VideoParser : public MediaParser
{
public:
    VideoParser(const xml_node& node);
    VideoOptions parse();
};
