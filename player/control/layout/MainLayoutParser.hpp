#pragma once

#include "constants.hpp"
#include "ParsedLayout.hpp"

#include <boost/property_tree/ptree.hpp>

class MainLayoutParser
{
public:
    ParsedLayout parse(const xml_node& node);

private:
    Uri getUri(const xml_node& node);
    uint32_t getColor(const xml_node& node);

    std::vector<ParsedRegion> parseRegions(const xml_node& node);
};

