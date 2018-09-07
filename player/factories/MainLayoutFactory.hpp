#pragma once

#include "constants.hpp"

#include <filesystem>
#include <boost/property_tree/ptree.hpp>

class IMainLayout;

class MainLayoutFactory
{
public:
    MainLayoutFactory(const xlf_node& layoutNode);
    std::unique_ptr<IMainLayout> create();

private:
    xlf_node m_layoutNode;

};
