#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>
#include <boost/property_tree/ptree.hpp>

class MainLayout;

class LayoutParser
{
public:
    LayoutParser(const xlf_node& layoutNode);
    std::unique_ptr<MainLayout> parse();
    std::vector<xlf_node>::const_iterator begin() const;
    std::vector<xlf_node>::const_iterator end() const;

private:
    xlf_node m_layoutNode;
    std::vector<xlf_node> m_regionNodes;

};
