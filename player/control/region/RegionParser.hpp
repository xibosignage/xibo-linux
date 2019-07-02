#pragma once

#include "constants.hpp"
#include "ParsedRegion.hpp"

#include <boost/property_tree/ptree.hpp>

class RegionParser
{
public: 
    ParsedRegion parse(const xml_node& node);

private:
    std::vector<ParsedMedia> parseMedia(int regionWidth, int regionHeight, const xml_node& node);
    MediaOptions::Type parseMediaType(const xml_node& node);

};

