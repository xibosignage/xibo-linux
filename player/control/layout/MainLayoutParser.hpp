#pragma once

#include "constants.hpp"
#include "MainLayoutOptions.hpp"

#include <boost/property_tree/ptree.hpp>

class MainLayoutParser
{
public:
    MainLayoutParser(const xml_node& node);
    MainLayoutOptions parse();

private:
    boost::property_tree::ptree m_layoutNode;

};

