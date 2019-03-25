#pragma once

#include "constants.hpp"
#include "Options.hpp"

#include <boost/property_tree/ptree.hpp>

class RegionsParser
{
public: 
    RegionsParser(const xml_node& regionNode);
    RegionOptions parse();

private:
    xml_node m_regionNode;

};

