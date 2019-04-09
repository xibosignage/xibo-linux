#pragma once

#include "constants.hpp"
#include "Options.hpp"

#include <boost/property_tree/ptree.hpp>

class MainLayoutParser
{
public:
    MainLayoutParser(const xml_node& node);
    LayoutOptions parse();

private:
    boost::property_tree::ptree m_layoutNode;

};

