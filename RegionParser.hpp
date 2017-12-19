#pragma once

#include "MainParser.hpp"
#include "Transition.hpp"
#include "constants.hpp"

using ParsedRegion = std::tuple<uint, Size, Point, int, bool, Transition>;

class RegionParser
{
public:
    RegionParser(const boost::property_tree::ptree& tree);
    ParsedRegion parse();

private:
    boost::property_tree::ptree m_tree;

};
