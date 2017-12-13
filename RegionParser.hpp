#pragma once

#include "MainParser.hpp"
#include "Transition.hpp"
#include "constants.hpp"

struct ParsedRegion
{
    int id;
    Size size;
    Point pos;
    int zindex;
    bool looped;
    Transition transition;
};

class RegionParser
{
public:
    RegionParser(const boost::property_tree::ptree& tree);
    ParsedRegion parse();

private:
    boost::property_tree::ptree m_tree;

};
