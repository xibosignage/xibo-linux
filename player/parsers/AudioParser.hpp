#pragma once

#include "MediaParser.hpp"

class AudioParser : public MediaParser
{
public:
    AudioParser(const xml_node& node);
    AudioOptions parse();
    AudioOptions parseNode();

};
