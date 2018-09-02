#pragma once

#include "constants.hpp"
#include <boost/property_tree/ptree.hpp>

class IRegion;

class RegionFactory
{
public:
    RegionFactory(const xlf_node& regionNode);
    std::unique_ptr<IRegion> create();

private:
    xlf_node m_regionNode;

};
