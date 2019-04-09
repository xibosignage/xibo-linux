#pragma once

#include "constants.hpp"
#include "RegionOptions.hpp"

#include <boost/property_tree/ptree.hpp>

class RegionParser
{
public: 
    RegionParser(const xml_node& regionNode);
    RegionOptions parse();

private:
    xml_node m_regionNode;

};

