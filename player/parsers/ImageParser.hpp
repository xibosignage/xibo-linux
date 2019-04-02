#pragma once

#include "MediaParser.hpp"

class ImageParser : public MediaParser
{
public:
    ImageParser(const xml_node& node);
    ImageOptions parse();

};
